#include "communicator.h"
#include "signaldata.h"

#include <QPointF>
#include <QDebug>

communicator::communicator(QObject *parent) : QThread(parent)
{
    p_portlist = new portlist();
    p_data.channel1 = new signaldata;
    p_data.channel2 = new signaldata;
    p_data.channel3 = new signaldata;
    p_data.channel4 = new signaldata;
}

void communicator::run()
{
    qDebug() << "[communicator]: measure loop started";
    p_stop = false;
    while( !p_stop )
    {
        qDebug() << "[communicator]: measure";
        measure();
        qDebug() << "[communicator]: measure finished, mapping data...";
        if( getchannelactive(CH1A) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel1->append(QPointF(1.0*i*(1/getsamplerate()),getvalue(i,CH1A)));
        if( getchannelactive(CH1B) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel2->append(QPointF(1.0*i*(1/getsamplerate()),getvalue(i,CH1B)));
        if( getchannelactive(CH2A) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel3->append(QPointF(1.0*i*(1/getsamplerate()),getvalue(i,CH2A)));
        if( getchannelactive(CH2B) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel4->append(QPointF(1.0*i*(1/getsamplerate()),getvalue(i,CH2B)));
        qDebug() << "[communicator]: mapping data finished, redraw!";
    }
}

void communicator::stop()
{
    qDebug() << "[communicator]: stop requested!";
    p_stop = true;
    stopmeasure();
}

signaldata* communicator::getdata(char channel)
{
    switch(channel) {
    case 0 : return p_data.channel1;
    case 1 : return p_data.channel2;
    case 2 : return p_data.channel3;
    case 3 : return p_data.channel4;
    default : return 0;
    }
}

bool communicator::openport(char *port)
{
    qDebug() << "[communicator]: opening port" << port;
    lastTriedPort() = QString::fromAscii(port);
    p_connected = lenboard::openport(port) >= 0;
    emit connectionStateChanged(p_connected);
    return p_connected;
}

bool communicator::closeport()
{
    qDebug() << "[communicator]: closing port...";
    p_connected = false;
    emit connectionStateChanged(p_connected);
    return lenboard::closeport() >= 0;
}

bool communicator::setsinusfrequency(unsigned int frequency)
{
    qDebug() << "[communicator]: set sinus frequency" << frequency;
    return lenboard::setsinusfrequency(frequency) == 0;
}

bool communicator::setsquarefrequency(unsigned int frequency)
{
    qDebug() << "[communicator]: set square frequency" << frequency;
    return lenboard::setsquarefrequency(frequency) == 0;
}

bool communicator::setsquareratio(int ratio)
{
    qDebug() << "[communicator]: set square ratio" << ratio;
    return lenboard::setsquareratio(ratio) == 0;
}

bool communicator::setactivechannels(int channels)
{
    qDebug() << "[communicator]: set active channels" << channels;
    return lenboard::setactivechannels(channels) == 0;
}

bool communicator::activatechannel(int channel, bool active)
{
    qDebug() << "[communicator]: activate channel" << channel << ":" << active;
    return lenboard::activatechannel(channel,active) == 0;
}

bool communicator::setvoltagedivision(int channel, int voltagedivision)
{
    qDebug() << "[communicator]: set channel" << channel << "voltagedivision" << voltagedivision;
    return lenboard::setvoltagedivision(channel,voltagedivision) == 0;
}

bool communicator::setoffsetchannel(int channel, bool active)
{
    qDebug() << "[communicator]: set channel" << channel << "offset" << active;
    return lenboard::setoffsetchannel(channel,active) == 0;
}

bool communicator::setoffset(int channels)
{
    qDebug() << "[communicator]: set offset" << channels;
    return lenboard::setoffset(channels) == 0;
}

bool communicator::setsamplerate(long unsigned int samplerate)
{
    qDebug() << "[communicator]: set sample rate" << samplerate;
    return lenboard::setsamplerate(samplerate) == 0;
}

bool communicator::setchannel1active(bool active)
{
    qDebug() << "[communicator]: set channel 1:" << active;
    return activatechannel(CH1A,active);
}

bool communicator::setchannel2active(bool active)
{
    qDebug() << "[communicator]: set channel 2:" << active;
    return activatechannel(CH1B,active);
}

bool communicator::setchannel3active(bool active)
{
    qDebug() << "[communicator]: set channel 3:" << active;
    return activatechannel(CH2A,active);
}

bool communicator::setchannel4active(bool active)
{
    qDebug() << "[communicator]: set channel 4:" << active;
    return activatechannel(CH2B,active);
}
