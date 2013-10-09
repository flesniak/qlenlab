/***************************************************************************
 * Copyright (C) 2011-2012 Fabian Lesniak <fabian.lesniak@student.kit.edu> *
 *                                                                         *
 * This file is part of the QLenLab project.                               *
 *                                                                         *
 * QLenLab is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * QLenLab is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with QLenLab. If not, see <http://www.gnu.org/licenses/>.               *
 **************************************************************************/

#include "communicator.h"
#include "signaldata.h"
#include "storage.h"

#include <QPointF>
#include <QDebug>
#include <QDateTime>

communicator::communicator(storage *datastorage, QObject *parent) : QThread(parent), p_storage(datastorage)
{
    p_portlist = new portlist();

    p_runmode = meta::none;
    p_connectstate = meta::disconnected;

    p_triggermode = meta::deactivated;
    p_triggerchannel = 0;

    p_activechannels = 0xF0;
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
    p_firstrun = true;

    p_bodetarget = 0;
    p_startFreq = 0;
    p_endFreq = 0;
    p_decadePoints = 0;

    p_manualoffset[0]=0.0;
    p_manualoffset[1]=0.0;
    p_manualoffset[2]=0.0;
    p_manualoffset[3]=0.0;

    qRegisterMetaType<meta::connectstate>("meta::connectstate");
    qRegisterMetaType<bodestate>("bodestate");
}

communicator::~communicator()
{
    if( isRunning() )
        stop();
    wait(17000000/getsamplerate());
    closeport();
}

bool communicator::setRunMode(meta::runmode mode)
{
    if( isRunning() )
        return false;
    else {
        p_runmode = mode;
        return true;
    }
}

meta::runmode communicator::runMode() const
{
    return p_runmode;
}

void communicator::run()
{
    qDebug() << "[communicator] thread started, mode" << p_runmode;
    switch( p_runmode ) {
    case meta::measure : doMeasure();
                         break;
    case meta::connect : doConnect();
                         break;
    case meta::bode    : doBodeDiagram();
    default            : break;
    }
}

void communicator::doConnect()
{
    if( !p_port.isEmpty() ) {
        if( p_connectstate == meta::connected )
            closeport();
        if( p_connectstate == meta::connected ) {
            qDebug() << "[communicator] unable to disconnect, not connecting!";
            return;
        }
        qDebug() << "[communicator] opening port" << p_port;
        setConnectState(meta::connecting);
        if( lenboard::openport(p_port.toAscii().data()) >= 0 )
            setConnectState(meta::connected);
        else {
            qDebug() << "[communicator] failed to open port" << p_port;
            setConnectState(meta::connectfail);
        }
    }
}

void communicator::doMeasure()
{
    emit measureStateChanged(true);
    p_stop = false;
    while( !p_stop )
    {
        setParameters();
        unsigned char actives = ((p_activechannels & 1) ? 1 : 0) + ((p_activechannels & 2) ? 1 : 0) + ((p_activechannels & 4) ? 1 : 0) + ((p_activechannels & 8) ? 1 : 0);
        if( actives == 0 ) {
            qDebug() << "[communicator] no channels active, stopping";
            p_stop = true;
            stopmeasure();
            break;
        }
        measure();

        unsigned char* buffer = getrawmeasurement();
        dataset newset;
        newset.timestamp = new QTime(QTime::currentTime());
        for(unsigned char i=0; i<4; i++) {
            newset.channel[i] = new signaldata;
            if( activechannels[i] )
                newset.channel[i]->reserve(16400/actives);
        }
        unsigned char channel = 255; //to get channel = 0 for index == 0
        for(int index = 0; index < getrawvaluecount(); index++) {
            do {
                if( channel < 3 )
                    channel++;
                else
                    channel = 0;
            } while( !activechannels[channel] );
            newset.channel[channel]->append(calcvalue(channel,buffer[index]));
        }

        for(int index=0;index<4;index++)
            if( newset.channel[index]->size() != 0 ) {
                newset.channel[index]->setInterval(1000.0/samplerate);
                newset.channel[index]->smooth(p_smoothFactor);
            }
        //do not delete empty datasets

        newset.channel[p_triggerchannel]->setTrigger(p_triggermode,p_triggervalue,3.3/256*getrangefactor(vdivision[channel/2]));
        for(int index=0;index<4;index++)
            if( newset.channel[index] != 0 && index != p_triggerchannel )
                newset.channel[index]->inheritTriggerOffset(newset.channel[p_triggerchannel]->getTriggerOffset());

        p_storage->appendDataset(newset);
        emit displayNewDataset();
    }
    emit measureStateChanged(false);
    qDebug() << "[communicator] measure loop stopped";
}

void communicator::doBodeDiagram()
{
    if( p_bodetarget == 0 ) {
        qDebug() << "[communicator] no bode target specified";
        return;
    }
    if( p_startFreq == 0 || p_endFreq == 0 || p_endFreq < p_startFreq ) {
        qDebug() << "[communicator] bode frequencies invalid";
        return;
    }
    if( p_decadePoints == 0 ) {
        qDebug() << "[communicator] zero decade points";
        return;
    }
    unsigned char portMask = p_bodeio[0]+p_bodeio[1];
    if( portMask <= 1 || portMask >= 11 || portMask == 3 ) {
        qDebug() << "[communicator] invalid channel mapping" << QString::number(portMask,2);
        return;
    }

    emit measureStateChanged(true);
    qDebug() << "[communicator] measure state true";
    bool finished = false;
    char doAgainCount = 0; //do not increase p_startFreq immediately
    p_stop = false;
    const unsigned char periodsPerMeasure = 30;
    p_activechannels = portMask | 0xF0;
    p_activechannels_changed = true;
    unsigned char invdiv = 3, outvdiv = 3;
    p_sinusfrequency = p_startFreq;
    double realSinus = p_sinusfrequency+0.1;
    p_bodetarget->setFreqs(p_startFreq,p_endFreq);
    qDebug() << "[communicator] main parameters set";

    while ( !finished && !p_stop )
    {
        p_voltagedivision = (invdiv << 2*(chan2num(p_bodeio[0])/2)) | (outvdiv << 2*(chan2num(p_bodeio[1])/2));
        qDebug() << "[communicator] p_voltdiv" << QString::number(p_voltagedivision,2);
        p_voltagedivision_changed = true;
        if( doAgainCount > 3 )
            doAgainCount = -1;
        if( doAgainCount < 0 ) {
            //Calculate next sinus frequency
            while( round(realSinus) <= p_sinusfrequency )
                realSinus *= pow(10,1.0/p_decadePoints);
            p_sinusfrequency = round(realSinus);
            p_sinusfrequency_changed = true;
            if( p_sinusfrequency > p_endFreq ) {
                finished = true;
                break;
            }
        }
        p_samplerate = ceil(16500.0*p_sinusfrequency/periodsPerMeasure/1000)*1000;
        if( p_samplerate > 200000 )
            p_samplerate = 200000;
        p_samplerate_changed = true;
        setParameters();

        qDebug() << "[communicator] measuring with: p_sinusfrequency" << p_sinusfrequency;
        qDebug() << "[communicator] measuring with: p_samplerate" << p_samplerate;

//#define DEBUGBODE
#ifdef DEBUGBODE
        dataset newset;
        newset.timestamp = new QTime(QTime::currentTime());
        newset.channel[0] = new signaldata;
        newset.channel[1] = new signaldata;
        newset.channel[2] = new signaldata;
        newset.channel[3] = new signaldata;
        newset.channel[0]->setTimeInterval(1000.0/samplerate);
        newset.channel[2]->setTimeInterval(1000.0/samplerate);
#endif

        measure();
        unsigned char* buffer = getrawmeasurement();
        bool channel = true; //true=output, false=input
        double inmax = 0, inmin = 0, outmax = 0, outmin = 0;
        for(int index = 0; index < getrawvaluecount()-1; index++) {
            channel = !channel;
            double value = calcvalue(channel?chan2num(p_bodeio[1]):chan2num(p_bodeio[0]),buffer[index]);
            if( channel ) {
                if( value > outmax )
                    outmax = value;
                if( value < outmin )
                    outmin = value;
            }
            else {
                if( value > inmax )
                    inmax = value;
                if( value < inmin )
                    inmin = value;
            }
#ifdef DEBUGBODE
            newset.channel[channel?2:0]->append(value);
#endif
            if( p_stop )
                break;
        }

        unsigned char old_invdiv = p_voltagedivision >> 2*(chan2num(p_bodeio[0])/2) & 3, old_outvdiv = p_voltagedivision >> 2*(chan2num(p_bodeio[1])/2) & 3;
        //Check range: case 1 - range too big
        if( (inmax > fabs(inmin) ? inmax : fabs(inmin)) < 1.58*getrangefactor(invdiv-1) ) {
            qDebug() << "[communicator] smaller range possible!";
            if( invdiv > 0 ) {
                invdiv--;
                doAgainCount++;
            }
            else
                doAgainCount = -1;
        } //case 2 - range too small
        else if( (inmax > fabs(inmin) ? inmax : fabs(inmin)) >= 1.58*getrangefactor(invdiv) ) {
            qDebug() << "[communicator] bigger range needed!";
            if( invdiv < 3 ) {
                invdiv++;
                doAgainCount++;
            }
            else
                doAgainCount = -1;
        }
        //Check range: case 1 - range too big
        if( (outmax > fabs(outmin) ? outmax : fabs(outmin)) < 1.58*getrangefactor(outvdiv-1) ) {
            qDebug() << "[communicator] smaller range possible!";
            if( outvdiv > 0 ) {
                outvdiv--;
                doAgainCount += doAgainCount < 0;
            }
        } //case 2 - range too small
        else if( (outmax > fabs(outmin) ? outmax : fabs(outmin)) >= 1.58*getrangefactor(outvdiv) ) {
            qDebug() << "[communicator] bigger range needed!";
            if( outvdiv < 3 ) {
                outvdiv++;
                doAgainCount += doAgainCount < 0;
            }
        }
        //case 3 - range okay
        if( doAgainCount == -1 ) {
            qDebug() << "[communicator] range okay";
            doAgainCount = -1;
            bodestate state;
            state.frequency = p_sinusfrequency;
            state.inputAmplitude = inmax-inmin;
            state.outputAmplitude = outmax-outmin;
            state.amplification = 20*log10(fabs(state.outputAmplitude/state.inputAmplitude));
            state.samplerate = p_samplerate;
            state.progress = round(100.0*realSinus/(p_endFreq-p_startFreq));
            p_bodetarget->append(QPointF(p_sinusfrequency,state.amplification));
            emit bodeStateUpdate(state);
            qDebug() << "[communicator] new bodestate: state.inputAmplitude" << state.inputAmplitude;
            qDebug() << "[communicator] new bodestate: state.outputAmplitude" << state.outputAmplitude;
            qDebug() << "[communicator] new bodestate: state.amplification" << state.amplification;
        }
        qDebug() << "[communicator] invdiv from" << QString::number(old_invdiv,2) << "to" << QString::number(invdiv,2);
        qDebug() << "[communicator] outvdiv from" << QString::number(old_outvdiv,2) << "to" << QString::number(outvdiv,2);
#ifdef DEBUGBODE
        p_storage->appendDataset(newset);
        emit displayNewDataset();
#endif
    }
    emit measureStateChanged(false);
    qDebug() << "[communicator] bode complete";
}

void communicator::setBodeParameters(bodedata *target, unsigned short startFreq, unsigned short endFreq, unsigned short decadePoints, meta::channel input, meta::channel output)
{
    qDebug() << "[communincator] bode parameter: target" << target;
    qDebug() << "[communincator] bode parameter: startFreq" << startFreq;
    qDebug() << "[communincator] bode parameter: endFreq" << endFreq;
    qDebug() << "[communincator] bode parameter: decadePoints" << decadePoints;
    qDebug() << "[communincator] bode parameter: input" << input;
    qDebug() << "[communincator] bode parameter: output" << output;
    p_bodetarget = target;
    p_startFreq = startFreq;
    p_endFreq = endFreq;
    p_decadePoints = decadePoints;
    p_bodeio[0] = input;
    p_bodeio[1] = output;
}

bodedata* communicator::bodeTarget() const
{
    return p_bodetarget;
}

double communicator::calcvalue(unsigned char channel, unsigned char raw)
{
    double value = raw;
    if( offsetchannels[channel] )
        value -= 127; //127; 127.5; 128???
    value *= 3.3/256;
    value *= getrangefactor( vdivision[channel/2] );
    //qDebug() << "[communicator] calcvalue channel" << QString::number(channel,2) << "rangefactor" << getrangefactor( vdivision[channel/2] );
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
    case 1 : return(1);
             break;
    case 2 : return(2);
             break;
    case 3 : return(10);
    default : return(0);
              break;
    }
}

meta::connectstate communicator::connectState() const
{
    return p_connectstate;
}

void communicator::setPort(QString port)
{
    p_port = port;
}

void communicator::stop()
{
    qDebug() << "[communicator] stop requested!";
    p_stop = true;
    stopmeasure();
    if( !wait(2000) ) {
        qDebug() << "[communicator] thread not stopping, terminating!";
        terminate();
    }
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

bool communicator::closeport() //don't supply threaded disconnect to ensure disconnecting is done on quitting
{
    qDebug() << "[communicator] closing port...";
    if( isRunning() )
        stop();
    if( lenboard::closeport() >= 0 ) {
        setConnectState(meta::disconnected);
        return true;
    }
    else
        return false;
}

void communicator::setConnectState(meta::connectstate state)
{
    if( state != p_connectstate ) {
        p_connectstate = state;
        emit connectionStateChanged(state);
    }
}

QString& communicator::port()
{
    return p_port;
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
    p_firstrun = false;
}

void communicator::settriggermode(meta::triggermode mode, double value, unsigned char channel)
{
    p_triggermode = mode;
    p_triggervalue = value;
    p_triggerchannel = channel; //store as plain number instead of meta::channel to save cpu time in doMeasure()
    qDebug() << "[communicator] set trigger mode" << mode << "value" << value << "channel" << channel;
}

void communicator::setsmoothfactor(const float smoothFactor)
{
    p_smoothFactor = smoothFactor;
    qDebug() << "[communicator] set smooth factor" << smoothFactor;
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
        qDebug() << "[communicator] set channel" << c << "offset" << active << "changed!";
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
