#ifndef FFTTHREAD_H
#define FFTTHREAD_H

#include <QThread>
#include <QQueue>
#include <fftw3.h>

#include "meta.h"

class signaldata;

/* fftthread starts to calculate the fft of a given data
 * and writes the result to inputData->getFft(). */

class fftthread : public QThread
{
    Q_OBJECT
public:
    explicit fftthread(QObject *parent = 0);
    virtual ~fftthread();
    dataset* getDataset();
    bool setDataset(dataset* ds);
    void stop();

protected:
    void run();

private:
    dataset* p_ds;
    fftw_plan p_plan;
    const unsigned int p_plansize;
    bool p_stop;
    double* p_input;
    double* p_output;
};

#endif // FFTTHREAD_H
