#include "fftthread.h"
#include "signaldata.h"

fftthread::fftthread(QObject* parent) : QThread(parent), p_plan(0), p_plansize(0)
{
}

fftthread::~fftthread()
{
    if( isRunning() ) {
        stop();
        if( !wait(200000) )
            terminate();
    }
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
        signaldata* input = p_ds.channel[i];
        if( input->size() == 0 )
            continue;

        double* output = fftw_alloc_real(input->rawSize()*sizeof(double));

        if( p_plan != 0 )
            while( p_plansize > input->rawSize() && input->rawSize() > p_plansize-100 ) //input is less than 100 values smaller than p_plansize, lets do zero-padding
                input->append(0);

        if( p_plan == 0 || p_plansize != input->rawSize() ) { //plan is not yet created or plan size differs from new data size
            qDebug() << "[fftthread] initializing new plan size" << input->rawSize() << "old size" << p_plansize;
            p_plansize = input->rawSize();
            double* testData = fftw_alloc_real(p_plansize*sizeof(double));
            memcpy(testData,input->rawData(),p_plansize);
            p_plan = fftw_plan_r2r_1d(p_plansize,testData,output,FFTW_R2HC,FFTW_MEASURE); //create a "good" plan to save time in each execute
            fftw_free(testData);
        }

        fftw_execute_r2r(p_plan, input->rawData(), output);

        input->setFft(new signaldata());
        double* target = input->getFft()->rawData(p_plansize); //rawData(size) resizes the data appropriately
        for(unsigned int i = 0; i < p_plansize; i++) {
            target[i] = sqrt(fabs(output[i])*fabs(output[i])+fabs(output[p_plansize-i])*fabs(output[p_plansize-i]))*2/p_plansize;
        }
        fftw_free(output);
        input->getFft()->setInterval(1000.0/p_plansize/input->getInterval()); //set frequency to input's fft
    }
}

void fftthread::stop()
{
    p_stop = true;
    qDebug() << "[fftthread] stop requested";
}
