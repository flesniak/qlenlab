#include "fftthread.h"
#include "signaldata.h"

fftthread::fftthread(QObject* parent) : QThread(parent), p_plan(0), p_plansize(65536)
{
    p_input = fftw_alloc_real(p_plansize*sizeof(double));
    p_output = fftw_alloc_real(p_plansize*sizeof(double));
}

fftthread::~fftthread()
{
    if( isRunning() ) {
        stop();
        if( !wait(200000) )
            terminate();
    }
    fftw_free(p_input);
    fftw_free(p_output);
}

dataset fftthread::getDataset() const
{
    return p_ds;
}

bool fftthread::setDataset(dataset ds)
{
    if( !isRunning() ) { //drop datasets if machine is still busy -> too slow
        p_ds = ds;
        return true;
    }
    return false;
}

void fftthread::run()
{
    p_stop = false;
    for(unsigned int i = 0; i < 4 && !p_stop; i++ ) {
        signaldata* inputData = p_ds.channel[i];
        if( inputData->size() == 0 )
            continue;

        memcpy(p_input,inputData->rawData(),inputData->size()*sizeof(double));
        memset(p_input+inputData->size(),0,(p_plansize-inputData->size())*sizeof(double)); //zero-padding

        if( p_plan == 0 ) { //plan is not yet created
            p_plan = fftw_plan_r2r_1d(p_plansize,p_input,p_output,FFTW_R2HC,FFTW_MEASURE); //create a "good" plan to save time in each execute
        }

        fftw_execute(p_plan);

        inputData->setFft(new signaldata());
        double* target = inputData->getFft()->rawData(p_plansize); //rawData(size) resizes the data appropriately
        for(unsigned int i = 0; i < p_plansize; i++) {
            target[i] = sqrt(p_output[i]*p_output[i]+p_output[p_plansize-i]*p_output[p_plansize-i])*2/inputData->size();
        }
        inputData->getFft()->setInterval(1000.0/p_plansize/inputData->getInterval()); //set frequency to input's fft
    }
}

void fftthread::stop()
{
    p_stop = true;
    qDebug() << "[fftthread] stop requested";
}
