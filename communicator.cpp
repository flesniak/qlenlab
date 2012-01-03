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

    p_activechannels = -1;
    p_activechannels_changed = false;
    p_channeloffset_changed = false;
    p_samplerate = -1;
    p_samplerate_changed = false;
    p_sinusfrequency = -1;
    p_sinusfrequency_changed = false;
    p_squarefrequency = -1;
    p_squarefrequency_changed = false;
    p_squareratio = -1;
    p_squareratio_changed = false;
}

void communicator::run()
{
    qDebug() << "[communicator] measure loop started";
    p_stop = false;
    while( !p_stop )
    {
        qDebug() << "[communicator] committing configuration";
        setParameters();
        qDebug() << "[communicator] measure";
        measure();
        qDebug() << "[communicator] measure finished, mapping data...";
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
        qDebug() << "[communicator] mapping data finished, redraw!";
        emit newDataset();
    }
}

void communicator::stop()
{
    qDebug() << "[communicator] stop requested!";
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
    qDebug() << "[communicator] opening port" << port;
    lastTriedPort() = QString::fromAscii(port);
    p_connected = lenboard::openport(port) >= 0;
    emit connectionStateChanged(p_connected);
    return p_connected;
}

bool communicator::closeport()
{
    qDebug() << "[communicator] closing port...";
    p_connected = false;
    emit connectionStateChanged(p_connected);
    return lenboard::closeport() >= 0;
}

int communicator::activechannelcount() const
{
    int v = 0;
    for(int i=1; i<=8; i*=2)
        if( getchannelactive(i) )
            v++;
    qDebug() << "[communicator] active channel count" << v;
    return v;
}

void communicator::setParameters()
{
    qDebug() << "[communicator] setting cached measure parameters";
    if( p_activechannels_changed )
        if( lenboard::setactivechannels( (p_activechannels & 1) ? ch1a : 0 + (p_activechannels & 2) ? ch1b : 0 + (p_activechannels & 4) ? ch2a : 0 + (p_activechannels & 8) ? ch2b : 0) )
            qDebug() << "[communicator] setting active channels failed";
    if( p_channeloffset_changed )
        if( lenboard::setoffset( (p_activechannels >> 4 & 1) ? ch1a : 0 + (p_activechannels >> 4 & 2) ? ch1b : 0 + (p_activechannels >> 4 & 4) ? ch2a : 0 + (p_activechannels >> 4 & 8) ? ch2b : 0) )
            qDebug() << "[communicator] setting channel offset failed";
    if( p_samplerate_changed )
        if( lenboard::setsamplerate(p_samplerate) )
            qDebug() << "[communicator] setting sample rate failed";
    if( p_sinusfrequency_changed)
        if( lenboard::setsinusfrequency(p_sinusfrequency) )
            qDebug() << "[communicator] setting sinus frequency failed";
    if( p_squarefrequency_changed)
        if( lenboard::setsquarefrequency(p_squarefrequency) )
            qDebug() << "[communicator] setting square frequency failed";
    if( p_squareratio_changed)
        if( lenboard::setsquareratio(p_squareratio) )
            qDebug() << "[communicator] setting square ratio failed";
    if( p_voltagedivision_changed )
        if( lenboard::setvoltagedivision(CH1,p_voltagedivision & 3) || lenboard::setvoltagedivision(CH2,p_voltagedivision >> 2 & 3) )
            qDebug() << "[communicator] setting voltagedivision failed";

}

bool communicator::setsinusfrequency(unsigned short frequency)
{
    qDebug() << "[communicator] set sinus frequency" << frequency;
    if( p_sinusfrequency != frequency ) {
        p_sinusfrequency = frequency;
        p_sinusfrequency_changed = true;
        return true;
        return true;
    }
    else
        return false;
}

bool communicator::setsquarefrequency(unsigned short frequency)
{
    qDebug() << "[communicator] set square frequency" << frequency;
    if( p_squarefrequency != frequency ) {
            p_squarefrequency = frequency;
            p_squarefrequency_changed = true;
            return true;
    }
    else
        return false;
}

bool communicator::setsquareratio(char ratio)
{
    qDebug() << "[communicator] set square ratio" << (int)ratio;
    if( p_squareratio != ratio ) {
        p_squareratio = ratio;
        p_squareratio_changed = true;
        return true;
    }
    else
        return false;
}

bool communicator::setactivechannels(char channels)
{
    qDebug() << "[communicator] set active channels" << (int)channels;
    if( p_activechannels != channels ) {
        p_activechannels = channels;
        p_activechannels_changed = true;
        return true;
    }
    else
        return false;
}

bool communicator::activatechannel(channel c, bool active)
{
    qDebug() << "[communicator] set channel" << c << (active ? "active" : "inactive");
    if( ((p_activechannels & c) > 0) != active ) {
        if( active )
            p_activechannels |= c;
        else
            p_activechannels &= ~c;
        p_activechannels_changed = true;
        return true;
    }
    else
        return false;
}

bool communicator::setvoltagedivision(channel c, int voltagedivision)
{
    qDebug() << "[communicator] set channel" << c << "voltagedivision" << voltagedivision;
    switch( c ) {
    case ch1 : p_voltagedivision &= voltagedivision;
               return true;
               break;
    case ch2 : p_voltagedivision &= voltagedivision*4;
               return true;
               break;
    default  : qDebug() << "[communicator] set voltage division: invalid channel specified";
               return false;
    }
}

void communicator::setrange1(int index)
{
    setvoltagedivision(ch1,index);
}

void communicator::setrange2(int index)
{
    setvoltagedivision(ch2,index);
}

bool communicator::setoffsetchannel(channel c, bool active)
{
    qDebug() << "[communicator] set channel" << c << "offset" << active;
    int c_i = c << 4;
    if( ((p_activechannels & c_i) > 0) != active ) {
        if( active )
            p_activechannels |= c_i;
        else
            p_activechannels &= ~c_i;
        p_channeloffset_changed = true;
        return true;
    }
    else
        return false;
}

bool communicator::setoffset(char channels)
{
    qDebug() << "[communicator] set active channel offset" << (int)channels;
    if( p_activechannels >> 4 != channels ) {
        p_activechannels = 4*channels;
        p_activechannels_changed = true;
        return true;
    }
    else
        return false;
}

bool communicator::setsamplerate(unsigned int samplerate)
{
    qDebug() << "[communicator] set sample rate" << samplerate;
    if( p_samplerate != samplerate ) {
        p_samplerate = samplerate;
        p_samplerate_changed = true;
        return true;
    }
    else
        return false;
}

bool communicator::setchannel1active(bool active)
{
    return activatechannel(ch1a,active);
}

bool communicator::setchannel2active(bool active)
{
    return activatechannel(ch1b,active);
}

bool communicator::setchannel3active(bool active)
{
    return activatechannel(ch2a,active);
}

bool communicator::setchannel4active(bool active)
{
    return activatechannel(ch2b,active);
}

bool communicator::setchannel1offset(bool active)
{
    return setoffsetchannel(ch1a,active);
}

bool communicator::setchannel2offset(bool active)
{
    return setoffsetchannel(ch1b,active);
}

bool communicator::setchannel3offset(bool active)
{
    return setoffsetchannel(ch2a,active);
}

bool communicator::setchannel4offset(bool active)
{
    return setoffsetchannel(ch2b,active);
}
