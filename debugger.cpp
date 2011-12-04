#include "debugger.h"
#include "ui_debugger.h"

debugger::debugger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::debugger)
{
    ui->setupUi(this);
}

debugger::~debugger()
{
    delete ui;
}
