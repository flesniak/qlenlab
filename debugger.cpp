#include "debugger.h"
#include "ui_debugger.h"

/* COMMAND INDEX:
no parameters:
0: 69 64 0D 0A + "id\n"
1: dso_arm
2: dso_stop

one parameter:
3: sig_sin
4: dso_oft
5: dso_div

two parameters:
6: sig_squ
7: dso_cfg */

debugger::debugger(QWidget *parent) : QDialog(parent), ui(new Ui::debugger)
{
    ui->setupUi(this);

    connect(ui->comboBox_command,SIGNAL(currentIndexChanged(int)),SLOT(update_editfields(int)));
    connect(ui->pushButton_send,SIGNAL(clicked()),SLOT(send_command()));
}

debugger::~debugger()
{
    delete ui;
}

void debugger::send_command()
{
    if( ui->comboBox_command->currentIndex() > 2 ) {
        if( ui->lineEdit_p1->text().isEmpty() )
            QMessageBox::critical(this,"Fehler","Parameter 1 darf nicht leer sein!");
        if( (ui->comboBox_command->currentIndex() > 5) & ui->lineEdit_p2->text().isEmpty() )
            QMessageBox::critical(this,"Fehler","Parameter 2 darf nicht leer sein!");
    }
    switch(ui->comboBox_command->currentIndex()) {
    case 0 : //do_something
             ui->textEdit_protocol->append("> 69 64 0D 0A + \"id\n\"");
             break;
    case 1 : //do_something
             ui->textEdit_protocol->append("> dso_arm");
             break;
    case 2 : //do_something
             ui->textEdit_protocol->append("> dso_stop");
             break;
    case 3 : //do_something
             ui->textEdit_protocol->append("> sig_sin " + ui->lineEdit_p1->text());
             break;
    case 4 : //do_something
             ui->textEdit_protocol->append("> dso_oft " + ui->lineEdit_p1->text());
             break;
    case 5 : //do_something
             ui->textEdit_protocol->append("> dso_div " + ui->lineEdit_p1->text());
             break;
    case 6 : //do_something
             ui->textEdit_protocol->append("> sig_squ " + ui->lineEdit_p1->text() + " " + ui->lineEdit_p2->text());
             break;
    case 7 : //do_something
             ui->textEdit_protocol->append("> dso_cfg " + ui->lineEdit_p1->text() + " " + ui->lineEdit_p2->text());
             break;
    }
}

void debugger::process_incoming(QByteArray data)
{
    ui->textEdit_protocol->append("< "+QString::fromLatin1(data));
}

void debugger::update_editfields(int index)
{
    if( index <= 2 ) {
        ui->lineEdit_p1->setEnabled(false);
        ui->lineEdit_p2->setEnabled(false);
    }
    if ( ( index > 2 ) & ( index < 6 ) ) {
        ui->lineEdit_p1->setEnabled(true);
        ui->lineEdit_p2->setEnabled(false);

    }
    if( index >= 6 ) {
        ui->lineEdit_p1->setEnabled(true);
        ui->lineEdit_p2->setEnabled(true);
    }
}
