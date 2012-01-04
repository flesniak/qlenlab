/************************************************************************
 * Copyright (C) 2011 Fabian Lesniak <fabian.lesniak@student.kit.edu>   *
 *                                                                      *
 * This file is part of the QLenLab project.                            *
 *                                                                      *
 * QLenLab is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * QLenLab is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with QLenLab. If not, see <http://www.gnu.org/licenses/>.      *
 ***********************************************************************/

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

#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>

dockWidget_test::dockWidget_test(QWidget *parent) : QDockWidget(parent)
{
    QWidget *testwidget = new QWidget;
    QGroupBox *box = new QGroupBox(this);
    box->setTitle("Textbox");
    QSpinBox *testbox = new QSpinBox(box);
    QLabel *testlabel = new QLabel("Testbox:",box);
    QHBoxLayout *layout = new QHBoxLayout(box);
    layout->addWidget(testlabel);
    layout->addWidget(testbox);
    QVBoxLayout *layout2 = new QVBoxLayout(testwidget);
    QLabel *testlabel2 = new QLabel("Text2",testwidget);
    layout2->addWidget(box);
    layout2->addWidget(testlabel2);
    setWidget(testwidget);
}

debugger::debugger(QWidget *parent) : QDialog(parent), ui(new Ui::debugger)
{
    ui->setupUi(this);

    connect(ui->comboBox_command,SIGNAL(currentIndexChanged(int)),SLOT(updateEditfields(int)));
    connect(ui->pushButton_send,SIGNAL(clicked()),SLOT(sendCommand()));
}

debugger::~debugger()
{
    delete ui;
}

void debugger::sendCommand()
{
    if( ui->comboBox_command->currentIndex() > 2 ) {
        if( ui->lineEdit_p1->text().isEmpty() )
            QMessageBox::critical(this,tr("Fehler"),tr("Parameter 1 darf nicht leer sein!"));
        if( (ui->comboBox_command->currentIndex() > 5) & ui->lineEdit_p2->text().isEmpty() )
            QMessageBox::critical(this,tr("Fehler"),tr("Parameter 2 darf nicht leer sein!"));
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

void debugger::processIncoming(QByteArray data)
{
    ui->textEdit_protocol->append("< "+QString::fromLatin1(data));
}

void debugger::updateEditfields(int index)
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
