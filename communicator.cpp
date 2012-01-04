/************************************************************************
 * Copyright (C) 2011 Fabian Lesniak <fabian.lesniak@student.kit.edu>   *
 *                                                                      *
 * This file is part of the QLenLab project.                            *
 *                                                                      *
 * QLenLab is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * QLenLab is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with QLenLab. If not, see <http://www.gnu.org/licenses/>.      *
 ***********************************************************************/

#include "communicator.h"
#include "signaldata.h"

#include <QPointF>
#include <QDebug>
#include <QDateTime>

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
    p_invert = 0;

    p_manualoffset[0]=0.0;
    p_manualoffset[1]=0.0;
    p_manualoffset[2]=0.0;
    p_manualoffset[3]=0.0;
}

communicator::~communicator()
{
    if( isRunning() )
        stop();
    wait(17000/getsamplerate());
    closeport();
}

void communicator::run()
{
    qDebug() << "[communicator] measure loop started";
    p_stop = false;
    while( !p_stop )
    {
        setParameters();
        measure();
        p_data.channel1->clear();
        p_data.channel2->clear();
        p_data.channel3->clear();
        p_data.channel4->clear();
        if( getchannelactive(ch1a) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel1->append(QPointF(1000.0/getsamplerate()*i,getvalue(i,ch1a)));
        if( getchannelactive(ch1b) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel2->append(QPointF(1000.0/getsamplerate()*i,getvalue(i,ch1b)));
        if( getchannelactive(ch2a) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel3->append(QPointF(1000.0/getsamplerate()*i,getvalue(i,ch2a)));
        if( getchannelactive(ch2b) )
            for(int i=0; i < getvaluecount(); i++)
                p_data.channel4->append(QPointF(1000.0/getsamplerate()*i,getvalue(i,ch2b)));
        emit newDataset();
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "[communicator] measurement complete";
    }
    qDebug() << "[communicator] measure loop stopped";
}

float communicator::getvalue(int number, channel c) const
{
    float value = lenboard::getvalue(number,(int)c);
    if( getoffset((int)c) > 0 )
        value -= 127; //127; 127.5; 128???
    value *= 3.3/256;
    switch( getoffset((int)c) ) {
    case 0 : value *= 0.5;
             break;
    case 2 : value *= 2;
             break;
    case 3 : value *= 10;
    default : break;
    }
    value += getmanualoffset(c);
    if( p_invert & c )
        value *= -1;
    return value;
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
    qDebug() << "[communicator] opening port" << (p_connected ? "successful" : "failed");
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
    if( p_activechannels_changed )
        if( lenboard::setactivechannels( p_activechannels & 0x0F ) )
            qDebug() << "[communicator] setting active channels failed";
    if( p_channeloffset_changed )
        if( lenboard::setoffset( p_activechannels >> 4 ) )
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
        if( lenboard::setvoltagedivision(CH1,p_voltagedivision & 3) | lenboard::setvoltagedivision(CH2,p_voltagedivision >> 2 & 3) )
            qDebug() << "[communicator] setting voltagedivision failed";

}

bool communicator::setsinusfrequency(unsigned short frequency)
{
    qDebug() << "[communicator] set sinus frequency" << frequency;
    if( p_sinusfrequency != frequency ) {
        p_sinusfrequency = frequency;
        p_sinusfrequency_changed = true;
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

bool communicator::setsquareratio(unsigned char ratio)
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

bool communicator::setactivechannels(unsigned char channels)
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

bool communicator::setvoltagedivision(channel c, unsigned char voltagedivision)
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
    unsigned char c_i = c << 4;
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

bool communicator::setoffset(unsigned char channels)
{
    qDebug() << "[communicator] set active channel offset" << (int)channels;
    if( p_activechannels >> 4 != channels ) {
        p_activechannels = channels << 4;
        p_activechannels_changed = true;
        return true;
    }
    else
        return false;
}

double communicator::getmanualoffset(channel c) const
{
    switch( c ) {
    case ch1a : return p_manualoffset[0];
    case ch1b : return p_manualoffset[1];
    case ch2a : return p_manualoffset[2];
    case ch2b : return p_manualoffset[3];
    default   : return 0.0;
    }
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


bool communicator::setmanualoffset1(double value)
{
    if( p_manualoffset[0] != value ) {
        qDebug() << "[communicator] set manual offset 1" << value;
        p_manualoffset[0] = value;
        return true;
    }
    else
        return false;
}

bool communicator::setmanualoffset2(double value)
{
    if( p_manualoffset[1] != value ) {
        qDebug() << "[communicator] set manual offset 2" << value;
        p_manualoffset[1] = value;
        return true;
    }
    else
        return false;
}

bool communicator::setmanualoffset3(double value)
{
    if( p_manualoffset[2] != value ) {
        qDebug() << "[communicator] set manual offset 3" << value;
        p_manualoffset[2] = value;
        return true;
    }
    else
        return false;
}

bool communicator::setmanualoffset4(double value)
{
    if( p_manualoffset[3] != value ) {
        qDebug() << "[communicator] set manual offset 4" << value;
        p_manualoffset[3] = value;
        return true;
    }
    else
        return false;
}

bool communicator::setchannel1invert(bool active)
{
    if( ((p_invert & 1) > 0) != active ) {
        qDebug() << "[communicator] set channel 1 invert" << active;
        if( active )
            p_invert |= 1;
        else
            p_invert &= ~1;
        return true;
    }
    else
        return false;
}

bool communicator::setchannel2invert(bool active)
{
    if( ((p_invert & 2) > 0) != active ) {
        qDebug() << "[communicator] set channel 2 invert" << active;
        if( active )
            p_invert |= 2;
        else
            p_invert &= ~2;
        return true;
    }
    else
        return false;
}

bool communicator::setchannel3invert(bool active)
{
    if( ((p_invert & 4) > 0) != active ) {
        qDebug() << "[communicator] set channel 3 invert" << active;
        if( active )
            p_invert |= 4;
        else
            p_invert &= ~4;
        return true;
    }
    else
        return false;
}

bool communicator::setchannel4invert(bool active)
{
    if( ((p_invert & 8) > 0) != active ) {
        qDebug() << "[communicator] set channel 4 invert" << active;
        if( active )
            p_invert |= 8;
        else
            p_invert &= ~8;
        return true;
    }
    else
        return false;
}
