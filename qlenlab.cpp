#include "qlenlab.h"
#include "ui_qlenlab.h"

QLenLab::QLenLab(QWidget *parent) : QMainWindow(parent), ui(new Ui::QLenLab)
{
    debug = NULL;

    ui->setupUi(this);

    connect(ui->action_debug,SIGNAL(triggered()),SLOT(show_debug()));
}

QLenLab::~QLenLab()
{
    delete ui;
}

void QLenLab::show_debug()
{
    if( debug == NULL ) {
        debug = new debugger(this);
        debug->show();
    }
    else
        debug->show();
}
