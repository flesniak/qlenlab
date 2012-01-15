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

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QThread>

#include "LENlib/LENlib.h"
#include "meta.h"

class signaldata;

class communicator : public QThread, public lenboard
{
    Q_OBJECT
public:
    communicator(QObject *parent = 0);
    ~communicator();
    portlist* portList() { return p_portlist; }
    QString& lastTriedPort() { return p_lastTriedPort; }
    bool openport(char *port);
    bool closeport();
    bool connected() { return p_connected; }
    signaldata* getdata(char channel);
    int activechannelcount() const;
    double getmanualoffset(meta::channel c) const;

public slots:
    void stop();

    //making functions available as Qt slots
    bool setsinusfrequency(unsigned short frequency);
    bool setsquarefrequency(unsigned short frequency);
    bool setsquareratio(unsigned char ratio);
    bool setactivechannels(unsigned char channels);
    bool activatechannel(meta::channel c, bool active);
    bool setvoltagedivision(meta::channel c, unsigned char voltagedivision);
    bool setoffsetchannel(meta::channel c, bool active);
    bool setoffset(unsigned char channels);
    bool setsamplerate(unsigned int samplerate);
    void settriggermode(meta::triggermode mode, double value);

    //some stuff for easier signal/slot handling
    bool setchannel1active(bool active);
    bool setchannel2active(bool active);
    bool setchannel3active(bool active);
    bool setchannel4active(bool active);
    bool setchannel1offset(bool active);
    bool setchannel2offset(bool active);
    bool setchannel3offset(bool active);
    bool setchannel4offset(bool active);
    bool setmanualoffset1(double value);
    bool setmanualoffset2(double value);
    bool setmanualoffset3(double value);
    bool setmanualoffset4(double value);
    bool setchannel1invert(bool active);
    bool setchannel2invert(bool active);
    bool setchannel3invert(bool active);
    bool setchannel4invert(bool active);
    void setrange1(int index);
    void setrange2(int index);

private:
    portlist* p_portlist;
    QString p_lastTriedPort;
    void setParameters();

protected:
    void run();
    float calcvalue(unsigned char channel, unsigned char raw);
    double getrangefactor(const unsigned char index) const;
    dataset p_data;
    bool p_connected;
    bool p_stop;
    unsigned char p_activechannels; // 4 lower bits = activechannels, 4 higher bits = channeloffset
    bool p_activechannels_changed;
    bool p_channeloffset_changed;
    unsigned int p_samplerate;
    bool p_samplerate_changed;
    unsigned short p_sinusfrequency;
    bool p_sinusfrequency_changed;
    unsigned short p_squarefrequency;
    bool p_squarefrequency_changed;
    unsigned char p_squareratio;
    bool p_squareratio_changed;
    unsigned char p_voltagedivision;
    bool p_voltagedivision_changed;
    double p_manualoffset[4];
    unsigned char p_invert;
    meta::triggermode p_triggermode;
    double p_triggervalue;

signals:
    void connectionStateChanged(bool);
    void measureStateChanged(bool);
    void newDataset();
};

#endif // COMMUNICATOR_H
