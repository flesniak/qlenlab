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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "communicator.h"

settingsdialog::settingsdialog(communicator* com, QWidget *parent) : QDialog(parent), ui(new Ui::settingsdialog), com(com)
{
    ui->setupUi(this);

    connect(ui->comboBox_serialport,SIGNAL(currentIndexChanged(int)),SLOT(updateConnectButton(int)));
    connect(ui->pushButton_rescan,SIGNAL(clicked()),SLOT(rescanDevices()));
    connect(ui->pushButton_connect,SIGNAL(clicked()),SLOT(connectSerial()));
    connect(ui->pushButton_disconnect,SIGNAL(clicked()),SLOT(disconnectSerial()));
    connect(ui->buttonBox,SIGNAL(accepted()),SLOT(accept()));
    connect(ui->buttonBox,SIGNAL(rejected()),SLOT(reject()));
}

settingsdialog::~settingsdialog()
{
    delete ui;
}

int settingsdialog::exec()
{
    return QDialog::exec();
}

void settingsdialog::restoreSettings()
{
    QSettings settings;
    ui->checkBox_autoconnect->setChecked(settings.value("serial/autoconnect",0).toBool());
    rescanDevices();
    QString device = settings.value("serial/port").toString();
    if( !device.isEmpty() )
    {
        int index = ui->comboBox_serialport->findText(device);
        if( index == -1 ) {
            ui->comboBox_serialport->addItem(device);
            ui->comboBox_serialport->setCurrentIndex(ui->comboBox_serialport->count()-1);
        }
        else
            ui->comboBox_serialport->setCurrentIndex(index);
        if( ui->checkBox_autoconnect->isChecked() )
            connectSerial();
    }
    ui->pushButton_disconnect->setEnabled(com->connected());
}

void settingsdialog::rescanDevices()
{
    ui->comboBox_serialport->clear();
    ui->comboBox_serialport->addItem(tr("Deaktiviert (debugging)"));
    com->portList()->refresh();
    char string[255];
    while(com->portList()->portwalk(string))
        ui->comboBox_serialport->addItem(string);
}

void settingsdialog::connectSerial()
{
    int index = ui->comboBox_serialport->currentIndex();
    if( index > 0 && ( (com->lastTriedPort() != ui->comboBox_serialport->currentText()) || !com->connected() ) ) //Only try to connect if we aren't already connected or change the device
    {
        com->closeport();
    //if( index > com->portList()->count() )
        com->openport(ui->comboBox_serialport->currentText().toAscii().data()); //This should work for both user-specified and detected ports.
    //else
    //    com->openport(com->portList()->portbynumber(index-1));
    }
    ui->pushButton_disconnect->setEnabled(com->connected());
}

void settingsdialog::disconnectSerial()
{
    com->closeport();
    ui->pushButton_disconnect->setEnabled(com->connected());
}

void settingsdialog::reject()
{
    QDialog::reject();
}

void settingsdialog::accept()
{
    QSettings settings;
    settings.setValue("serial/port", !com->lastTriedPort().isEmpty() ? com->lastTriedPort() : ui->comboBox_serialport->currentText());
    settings.setValue("serial/autoconnect",ui->checkBox_autoconnect->isEnabled() ? ui->checkBox_autoconnect->isChecked() : false);
    QDialog::accept();
}

void settingsdialog::updateConnectButton(int index)
{
    ui->pushButton_connect->setEnabled(index != 0);
    ui->checkBox_autoconnect->setEnabled(index != 0);
}
