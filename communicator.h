#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QThread>

#include "LENlib/LENlib.h"

class signaldata;

class communicator : public QThread, public lenboard
{
    Q_OBJECT
public:
    static communicator& instance();
    portlist* portList() {
        return p_portlist;
    }

private:
    explicit communicator(QObject *parent = 0);
    portlist* p_portlist;
    signaldata* data;

protected:
    void run();

signals:

public slots:

};

#endif // COMMUNICATOR_H
