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

settingsdialog::settingsdialog(QWidget *parent) : QDialog(parent), ui(new Ui::settingsdialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_rescan,SIGNAL(clicked()),SLOT(rescanDevices()));
    connect(ui->pushButton_connect,SIGNAL(clicked()),SLOT(connectSerial()));
    connect(ui->buttonBox,SIGNAL(accepted()),SLOT(accept()));
    connect(ui->buttonBox,SIGNAL(rejected()),SLOT(reject()));
}

settingsdialog::~settingsdialog()
{
    delete ui;
}

int settingsdialog::exec()
{
    rescanDevices();
    QSettings settings;
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
    }
    return QDialog::exec();
}

void settingsdialog::rescanDevices()
{

}

void settingsdialog::connectSerial()
{

}

void settingsdialog::reject()
{
    QDialog::reject();
}

void settingsdialog::accept()
{
    //set new device to communicator class
    connectSerial();
    //if successful:
    if( true ) {
        QSettings settings;
        settings.setValue("serial/port",ui->comboBox_serialport->currentText());
    }
    QDialog::accept();
}
