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

    p_activechannels = 0;
    p_activechannels_changed = false;
    p_channeloffset_changed = false;
    p_samplerate = 0;
    p_samplerate_changed = false;
    p_sinusfrequency = 0;
    p_sinusfrequency_changed = false;
    p_squarefrequency = 0;
    p_squarefrequency_changed = false;
    p_squareratio = 0;
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
    wait(17000000/getsamplerate());
    closeport();
}

void communicator::run()
{
    qDebug() << "[communicator] measure loop started";
    emit measureStateChanged(true);
    p_stop = false;
    while( !p_stop )
    {
        setParameters();
        unsigned char actives = ((p_activechannels & 1) ? 1 : 0) + ((p_activechannels & 2) ? 1 : 0) + ((p_activechannels & 4) ? 1 : 0) + ((p_activechannels & 8) ? 1 : 0);
        if( actives == 0 ) {
            stop();
            break;
        }
        measure();

        p_data.channel1->clear();
        p_data.channel2->clear();
        p_data.channel3->clear();
        p_data.channel4->clear();

        unsigned char* buffer = getrawmeasurement();
        signaldata *data[4];
        data[0]=p_data.channel1;
        data[1]=p_data.channel2;
        data[2]=p_data.channel3;
        data[3]=p_data.channel4;
        unsigned char channel = -1;
        for(int index = 0; index < getrawvaluecount(); index++) {
            do {
                if( channel < 3 )
                    channel++;
                else
                    channel = 0;
            } while( !activechannels[channel] );
            data[channel]->append(QPointF(1000.0/samplerate*(index/actives),calcvalue(channel,buffer[index])));
        }
        for(int index=0;index<4;index++)
            data[index]->setTrigger(p_triggermode,p_triggervalue,3.3/256*getrangefactor(vdivision[index/2]));
        emit newDataset();
        usleep(300);
    }
    emit measureStateChanged(false);
    qDebug() << "[communicator] measure loop stopped";
}

float communicator::calcvalue(unsigned char channel, unsigned char raw)
{
    float value = raw;
    if( offsetchannels[channel] )
        value -= 127; //127; 127.5; 128???
    value *= 3.3/256;
    value *= getrangefactor( vdivision[channel/2] );
    value += p_manualoffset[channel];
    if( (p_invert >> channel) & 1 )
        value *= -1;
    return value;
}

double communicator::getrangefactor(const unsigned char index) const
{
    switch( index ) {
    case 0 : return(0.5);
             break;
    case 2 : return(2);
             break;
    case 3 : return(10);
    default : return(1);
              break;
    }
}

void communicator::stop()
{
    qDebug() << "[communicator] stop requested!";
    p_stop = true;
    if( !wait(2000) )
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
    if( p_activechannels_changed ) {
        if( lenboard::setactivechannels( p_activechannels & 0x0F ) )
            qDebug() << "[communicator] setting active channels failed";
        p_activechannels_changed = false;
    }
    if( p_channeloffset_changed ) {
        if( lenboard::setoffset( p_activechannels >> 4 ) )
            qDebug() << "[communicator] setting channel offset failed";
        p_channeloffset_changed = false;
    }
    if( p_samplerate_changed ) {
        if( lenboard::setsamplerate(p_samplerate) )
            qDebug() << "[communicator] setting sample rate failed";
        p_samplerate_changed = false;
    }
    if( p_sinusfrequency_changed) {
        if( lenboard::setsinusfrequency(p_sinusfrequency) )
            qDebug() << "[communicator] setting sinus frequency failed";
        p_sinusfrequency_changed = false;
    }
    if( p_squarefrequency_changed) {
        if( lenboard::setsquarefrequency(p_squarefrequency) )
            qDebug() << "[communicator] setting square frequency failed";
        p_squarefrequency_changed = false;
    }
    if( p_squareratio_changed) {
        if( lenboard::setsquareratio(p_squareratio) )
            qDebug() << "[communicator] setting square ratio failed";
        p_squareratio_changed = false;
    }
    if( p_voltagedivision_changed ) {
        if( lenboard::setvoltagedivision(CH1,p_voltagedivision & 3) | lenboard::setvoltagedivision(CH2,p_voltagedivision >> 2 & 3) )
            qDebug() << "[communicator] setting voltagedivision failed";
        p_voltagedivision_changed = false;
    }
}

void communicator::settriggermode(meta::triggermode mode, double value)
{
    p_triggermode = mode;
    p_triggervalue = value;
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

bool communicator::activatechannel(meta::channel c, bool active)
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

bool communicator::setvoltagedivision(meta::channel c, unsigned char voltagedivision)
{
    qDebug() << "[communicator] set channel" << c << "voltagedivision" << voltagedivision;
    switch( c ) {
    case meta::ch1 : p_voltagedivision = (voltagedivision & 0b00000011) | (p_voltagedivision & 0b11111100);
                     p_voltagedivision_changed = true;
                     return true;
                     break;
    case meta::ch2 : p_voltagedivision = ((voltagedivision*4) & 0b00001100) | (p_voltagedivision & 0b11110011);
                     p_voltagedivision_changed = true;
                     return true;
                     break;
    default  : qDebug() << "[communicator] set voltage division: invalid channel specified";
               return false;
    }
}

void communicator::setrange1(int index)
{
    setvoltagedivision(meta::ch1,index);
}

void communicator::setrange2(int index)
{
    setvoltagedivision(meta::ch2,index);
}

bool communicator::setoffsetchannel(meta::channel c, bool active)
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

double communicator::getmanualoffset(meta::channel c) const
{
    switch( c ) {
    case meta::ch1a : return p_manualoffset[0];
    case meta::ch1b : return p_manualoffset[1];
    case meta::ch2a : return p_manualoffset[2];
    case meta::ch2b : return p_manualoffset[3];
    default         : return 0.0;
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
    return activatechannel(meta::ch1a,active);
}

bool communicator::setchannel2active(bool active)
{
    return activatechannel(meta::ch1b,active);
}

bool communicator::setchannel3active(bool active)
{
    return activatechannel(meta::ch2a,active);
}

bool communicator::setchannel4active(bool active)
{
    return activatechannel(meta::ch2b,active);
}

bool communicator::setchannel1offset(bool active)
{
    return setoffsetchannel(meta::ch1a,active);
}

bool communicator::setchannel2offset(bool active)
{
    return setoffsetchannel(meta::ch1b,active);
}

bool communicator::setchannel3offset(bool active)
{
    return setoffsetchannel(meta::ch2a,active);
}

bool communicator::setchannel4offset(bool active)
{
    return setoffsetchannel(meta::ch2b,active);
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
