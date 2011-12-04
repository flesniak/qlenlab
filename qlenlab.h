#ifndef QLENLAB_H
#define QLENLAB_H

#include <QMainWindow>

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
    Ui::QLenLab *ui;
};

#endif // QLENLAB_H
