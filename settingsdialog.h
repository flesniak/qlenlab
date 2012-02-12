/***************************************************************************
 * Copyright (C) 2011-2012 Fabian Lesniak <fabian.lesniak@student.kit.edu> *
 *                                                                         *
 * This file is part of the QLenLab project.                               *
 *                                                                         *
 * QLenLab is free software: you can redistribute it and/or modify it      *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * QLenLab is distributed in the hope that it will be useful, but WITHOUT  *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with QLenLab. If not, see <http://www.gnu.org/licenses/>.               *
 **************************************************************************/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>

#include "meta.h"

class communicator;
class QPalette;

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
    QColor getChannelColor(meta::channel c);

private:
    communicator* com;

    QComboBox *comboBox_serialport;
    QPushButton *pushButton_connect;
    QPushButton *pushButton_disconnect;
    QCheckBox *checkBox_autoconnect;

    QLabel *label_colorBackground;
    QLabel *label_colorGrid;
    QLabel *label_colorChannel1;
    QLabel *label_colorChannel2;
    QLabel *label_colorChannel3;
    QLabel *label_colorChannel4;
    QPushButton *pushButton_colorBackground;
    QPushButton *pushButton_colorGrid;
    QPushButton *pushButton_colorChannel1;
    QPushButton *pushButton_colorChannel2;
    QPushButton *pushButton_colorChannel3;
    QPushButton *pushButton_colorChannel4;
    QColor channelColor[6];

    char p_connectTries;

    void updateColor(meta::colorindex ci, QColor color);

private slots:
    void rescanDevices();
    void accept();
    void reject();
    void getColorBackground();
    void getColorGrid();
    void getColorChannel1();
    void getColorChannel2();
    void getColorChannel3();
    void getColorChannel4();
    void updateButtons();

signals:
    void colorChanged(meta::colorindex, QColor);
};

#endif // SETTINGSDIALOG_H
