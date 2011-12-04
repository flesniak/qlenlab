#include "qlenlab.h"
#include "ui_qlenlab.h"

QLenLab::QLenLab(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QLenLab)
{
    ui->setupUi(this);
}

QLenLab::~QLenLab()
{
    delete ui;
}
