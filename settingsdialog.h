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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QString>
#include <QSettings>

class communicator;

namespace Ui {
    class settingsdialog;
}

class settingsdialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingsdialog(communicator* com, QWidget *parent = 0);
    ~settingsdialog();

public slots:
    int exec();
    void connectSerial();
    void disconnectSerial();
    void restoreSettings();

private:
    Ui::settingsdialog *ui;
    communicator* com;

private slots:
    void rescanDevices();
    void accept();
    void reject();
    void updateConnectButton(int);
};

#endif // SETTINGSDIALOG_H
