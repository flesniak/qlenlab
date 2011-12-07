#include "communicator.h"

#include "signaldata.h"
#include <QDebug>
communicator::communicator(signaldata *data, QObject *parent) : QThread(parent), data(data)
{
    p_portlist = new portlist();
}

void communicator::run()
{
    //hand samples to signaldata
}

bool communicator::openport(char *port)
{
    lastTriedPort() = QString::fromAscii(port);
    p_connected = lenboard::openport(port) >= 0;
    emit connectionStateChanged(p_connected);
    return p_connected;
}

bool communicator::closeport()
{
    p_connected = false;
    emit connectionStateChanged(p_connected);
    return lenboard::closeport() >= 0;
}

bool communicator::setsinusfrequency(unsigned int frequency)
{
    return lenboard::setsinusfrequency(frequency) == 0;
}

bool communicator::setsquarefrequency(unsigned int frequency)
{
    return lenboard::setsquarefrequency(frequency) == 0;
}

bool communicator::setsquareratio(int ratio)
{
    return lenboard::setsquareratio(ratio) == 0;
}

bool communicator::setactivechannels(int channels)
{
    return lenboard::setactivechannels(channels) == 0;
}

bool communicator::activatechannel(int channel, bool active)
{
    return lenboard::activatechannel(channel,active) == 0;
}

bool communicator::setvoltagedivision(int channel, int voltagedevision)
{
    return lenboard::setvoltagedivision(channel,voltagedevision) == 0;
}

bool communicator::setoffsetchannel(int channel, bool active)
{
    return lenboard::setoffsetchannel(channel,active) == 0;
}

bool communicator::setoffset(int channels)
{
    return lenboard::setoffset(channels) == 0;
}

bool communicator::setsamplerate(long unsigned int samplerate)
{
    return lenboard::setsamplerate(samplerate) == 0;
}

bool communicator::setchannel1active(bool active)
{
    return activatechannel(CH1A,active);
}

bool communicator::setchannel2active(bool active)
{
    return activatechannel(CH1B,active);
}

bool communicator::setchannel3active(bool active)
{
    return activatechannel(CH2A,active);
}

bool communicator::setchannel4active(bool active)
{
    return activatechannel(CH2B,active);
}
