#ifndef QLENLAB_H
#define QLENLAB_H

#include <QMainWindow>

#include "debugger.h"

namespace Ui {
    class QLenLab;
}

class QLenLab : public QMainWindow
{
    Q_OBJECT

public:
    explicit QLenLab(QWidget *parent = 0);
    ~QLenLab();

private:
    debugger *debug;
    Ui::QLenLab *ui;

private slots:
    void show_debug();
};

#endif // QLENLAB_H
