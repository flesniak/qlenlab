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
    int activechannelcount() const;

public slots:
    void stop();

    //making functions available as Qt slots
    bool setsinusfrequency(unsigned short frequency);
    bool setsquarefrequency(unsigned short frequency);
    bool setsquareratio(char ratio);
    bool setactivechannels(char channels);
    bool activatechannel(channel c, bool active);
    bool setvoltagedivision(channel c, int voltagedivision);
    bool setoffsetchannel(channel c, bool active);
    bool setoffset(char channels);
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
    dataset p_data;
    bool p_connected;
    bool p_stop;
    char p_activechannels; // 4 lower bytes = activechannels, 4 higher bytes = channeloffset
    bool p_activechannels_changed;
    bool p_channeloffset_changed;
    unsigned int p_samplerate;
    bool p_samplerate_changed;
    unsigned short p_sinusfrequency;
    bool p_sinusfrequency_changed;
    unsigned short p_squarefrequency;
    bool p_squarefrequency_changed;
    char p_squareratio;
    bool p_squareratio_changed;
    char p_voltagedivision;
    bool p_voltagedivision_changed;

signals:
    void connectionStateChanged(bool);
    void newDataset();
};

#endif // COMMUNICATOR_H
