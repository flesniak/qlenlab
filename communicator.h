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

public slots:
    void stop();

    //making functions available as Qt slots
    bool setsinusfrequency(unsigned short frequency);
    bool setsquarefrequency(unsigned short frequency);
    bool setsquareratio(unsigned char ratio);
    bool setactivechannels(unsigned char channels);
    bool activatechannel(channel c, bool active);
    bool setvoltagedivision(channel c, unsigned char voltagedivision);
    bool setoffsetchannel(channel c, bool active);
    bool setoffset(unsigned char channels);
    bool setsamplerate(unsigned int samplerate);

    //some stuff for easier signal/slot handling
    bool setchannel1active(bool active);
    bool setchannel2active(bool active);
    bool setchannel3active(bool active);
    bool setchannel4active(bool active);
    bool setchannel1offset(bool active);
    bool setchannel2offset(bool active);
    bool setchannel3offset(bool active);
    bool setchannel4offset(bool active);
    void setrange1(int index);
    void setrange2(int index);

private:
    portlist* p_portlist;
    QString p_lastTriedPort;
    void setParameters();

protected:
    void run();
    float getvalue(int number, channel c) const;
    dataset p_data;
    bool p_connected;
    bool p_stop;
    unsigned char p_activechannels; // 4 lower bytes = activechannels, 4 higher bytes = channeloffset
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

signals:
    void connectionStateChanged(bool);
    void newDataset();
};

#endif // COMMUNICATOR_H
