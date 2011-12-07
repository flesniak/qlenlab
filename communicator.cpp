#include "communicator.h"

#include "signaldata.h"

communicator::communicator(QObject *parent) : QThread(parent)
{
    p_portlist = new portlist();
}

void communicator::run()
{
    //hand samples to signaldata
}

communicator& communicator::instance()
{
    static communicator& p_instance;
    return p_instance;
}
