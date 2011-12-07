#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QThread>

#include "LENlib/LENlib.h"

class signaldata;

class communicator : public QThread, public lenboard
{
    Q_OBJECT
public:
    communicator(signaldata* data, QObject *parent = 0);
    portlist* portList() { return p_portlist; }
    QString& lastTriedPort() { return p_lastTriedPort; }
    bool openport(char *port);
    bool closeport();
    bool connected() { return p_connected; }

public slots:
    //making functions available as Qt slots
    bool setsinusfrequency(unsigned int frequency);
    bool setsquarefrequency(unsigned int frequency);
    bool setsquareratio(int ratio);
    bool setactivechannels(int channels);
    bool activatechannel(int channel, bool active);
    bool setvoltagedivision(int channel, int voltagedevision);
    bool setoffsetchannel(int channel, bool active);
    bool setoffset(int channels);
    bool setsamplerate(long unsigned int samplerate);

    //some stuff for easier signal/slot handling
    bool setchannel1active(bool active);
    bool setchannel2active(bool active);
    bool setchannel3active(bool active);
    bool setchannel4active(bool active);

private:
    portlist* p_portlist;
    signaldata* data;
    QString p_lastTriedPort;
    bool p_connected;

protected:
    void run();

signals:
    void connectionStateChanged(bool);

public slots:

};

#endif // COMMUNICATOR_H
