#include <QtGui/QApplication>
#include "qlenlab.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLenLab w;
    w.show();

    return a.exec();
}
