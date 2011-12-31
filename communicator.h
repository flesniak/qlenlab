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
    portlist* portList() { return p_portlist; }
    QString& lastTriedPort() { return p_lastTriedPort; }
    bool openport(char *port);
    bool closeport();
    bool connected() { return p_connected; }
    signaldata* getdata(char channel);

public slots:
    void stop();

    //making functions available as Qt slots
    bool setsinusfrequency(unsigned int frequency);
    bool setsquarefrequency(unsigned int frequency);
    bool setsquareratio(int ratio);
    bool setactivechannels(int channels);
    bool activatechannel(int channel, bool active);
    bool setvoltagedivision(int channel, int voltagedivision);
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
    dataset p_data;
    QString p_lastTriedPort;
    bool p_connected;
    bool p_stop;

protected:
    void run();

signals:
    void connectionStateChanged(bool);
};

#endif // COMMUNICATOR_H
