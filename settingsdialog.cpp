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

#include "settingsdialog.h"
#include "communicator.h"

#include <QPalette>

settingsdialog::settingsdialog(communicator* com, QWidget *parent) : QDialog(parent), com(com)
{
    setWindowTitle(tr("Einstellungen"));
    p_connectTries = -1;

    QGroupBox *box_serialport = new QGroupBox(this);
    box_serialport->setTitle(tr("Serielle Schnittstelle"));

    comboBox_serialport = new QComboBox(box_serialport);
    comboBox_serialport->setEditable(true);
    pushButton_connect = new QPushButton(tr("Verbinden"),box_serialport);
    pushButton_disconnect = new QPushButton(tr("Verbindung trennen"),box_serialport);
    QPushButton *pushButton_rescan = new QPushButton(tr("Geräte aktualisieren"),box_serialport);
    checkBox_autoconnect = new QCheckBox(tr("Beim Start automatisch verbinden"),box_serialport);

    QGridLayout *layout_serialport = new QGridLayout(box_serialport);
    layout_serialport->addWidget(comboBox_serialport,0,0,1,2);
    layout_serialport->addWidget(pushButton_rescan,1,0,1,2);
    layout_serialport->addWidget(pushButton_connect,2,0);
    layout_serialport->addWidget(pushButton_disconnect,2,1);
    layout_serialport->addWidget(checkBox_autoconnect,3,0,1,2);

    QGroupBox *box_colors = new QGroupBox(this);
    box_colors->setTitle(tr("Farben im Plot"));

    QLabel *label_colorBackground = new QLabel(tr("Hintergrund"),box_colors);
    QLabel *label_colorGrid = new QLabel(tr("Raster"),box_colors);
    QLabel *label_colorChannel1 = new QLabel(tr("Kanal 1"),box_colors);
    QLabel *label_colorChannel2 = new QLabel(tr("Kanal 2"),box_colors);
    QLabel *label_colorChannel3 = new QLabel(tr("Kanal 3"),box_colors);
    QLabel *label_colorChannel4 = new QLabel(tr("Kanal 4"),box_colors);

    pushButton_colorBackground = new QPushButton(box_colors);
    pushButton_colorGrid = new QPushButton(box_colors);
    pushButton_colorChannel1 = new QPushButton(box_colors);
    pushButton_colorChannel2 = new QPushButton(box_colors);
    pushButton_colorChannel3 = new QPushButton(box_colors);
    pushButton_colorChannel4 = new QPushButton(box_colors);

    QGridLayout *layout_colors = new QGridLayout(box_colors);
    layout_colors->addWidget(label_colorBackground,0,0);
    layout_colors->addWidget(pushButton_colorBackground,0,1);
    layout_colors->addWidget(label_colorGrid,0,2);
    layout_colors->addWidget(pushButton_colorGrid,0,3);
    layout_colors->addWidget(label_colorChannel1,1,0);
    layout_colors->addWidget(pushButton_colorChannel1,1,1);
    layout_colors->addWidget(label_colorChannel2,1,2);
    layout_colors->addWidget(pushButton_colorChannel2,1,3);
    layout_colors->addWidget(label_colorChannel3,2,0);
    layout_colors->addWidget(pushButton_colorChannel3,2,1);
    layout_colors->addWidget(label_colorChannel4,2,2);
    layout_colors->addWidget(pushButton_colorChannel4,2,3);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box_serialport);
    layout->addWidget(box_colors);
    layout->addWidget(buttonBox);

    qRegisterMetaType<meta::colorindex>("meta::colorindex");

    connect(comboBox_serialport,SIGNAL(currentIndexChanged(int)),SLOT(updateButtons()));
    connect(pushButton_rescan,SIGNAL(clicked()),SLOT(rescanDevices()));
    connect(pushButton_connect,SIGNAL(clicked()),SLOT(connectSerial()));
    connect(pushButton_disconnect,SIGNAL(clicked()),SLOT(disconnectSerial()));
    connect(buttonBox,SIGNAL(accepted()),SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),SLOT(reject()));

    connect(pushButton_colorBackground,SIGNAL(clicked()),SLOT(getColorBackground()));
    connect(pushButton_colorGrid,SIGNAL(clicked()),SLOT(getColorGrid()));
    connect(pushButton_colorChannel1,SIGNAL(clicked()),SLOT(getColorChannel1()));
    connect(pushButton_colorChannel2,SIGNAL(clicked()),SLOT(getColorChannel2()));
    connect(pushButton_colorChannel3,SIGNAL(clicked()),SLOT(getColorChannel3()));
    connect(pushButton_colorChannel4,SIGNAL(clicked()),SLOT(getColorChannel4()));

    connect(com,SIGNAL(connectionStateChanged(meta::connectstate)),SLOT(updateButtons()));
}

settingsdialog::~settingsdialog()
{
}

int settingsdialog::exec()
{
    return QDialog::exec();
}

void settingsdialog::restoreSettings()
{
    QSettings settings;
    channelColor[meta::channel1] = settings.value("plot/color1",QColor(255,255,0)).value<QColor>();
    channelColor[meta::channel2] = settings.value("plot/color2",QColor(0,0,255)).value<QColor>();
    channelColor[meta::channel3] = settings.value("plot/color3",QColor(255,0,0)).value<QColor>();
    channelColor[meta::channel4] = settings.value("plot/color4",QColor(0,255,0)).value<QColor>();
    channelColor[meta::background] = settings.value("plot/colorBackground",QColor(0,0,0)).value<QColor>();
    channelColor[meta::grid] = settings.value("plot/colorGrid",QColor(255,255,255)).value<QColor>();
    updateColor(meta::channel1,channelColor[meta::channel1]);
    updateColor(meta::channel2,channelColor[meta::channel2]);
    updateColor(meta::channel3,channelColor[meta::channel3]);
    updateColor(meta::channel4,channelColor[meta::channel4]);
    updateColor(meta::background,channelColor[meta::background]);
    updateColor(meta::grid,channelColor[meta::grid]);
    emit colorChanged(meta::channel1,channelColor[meta::channel1]);
    emit colorChanged(meta::channel2,channelColor[meta::channel2]);
    emit colorChanged(meta::channel3,channelColor[meta::channel3]);
    emit colorChanged(meta::channel4,channelColor[meta::channel4]);
    emit colorChanged(meta::background,channelColor[meta::background]);
    emit colorChanged(meta::grid,channelColor[meta::grid]);

    checkBox_autoconnect->setChecked(settings.value("serial/autoconnect",0).toBool());
    rescanDevices();
    QString device = settings.value("serial/port").toString();
    if( !device.isEmpty() ) {
        int index = comboBox_serialport->findText(device);
        if( index == -1 ) {
            comboBox_serialport->addItem(device);
            comboBox_serialport->setCurrentIndex(comboBox_serialport->count()-1);
        }
        else
            comboBox_serialport->setCurrentIndex(index);
        if( checkBox_autoconnect->isChecked() )
            p_connectTries = 0;
    }
    updateButtons();
}

void settingsdialog::rescanDevices()
{
    comboBox_serialport->clear();
    comboBox_serialport->addItem(tr("Deaktiviert"));
    com->portList()->refresh();
    char string[255];
    while(com->portList()->portwalk(string))
        comboBox_serialport->addItem(string);
}

void settingsdialog::connectSerial()
{
    int index = comboBox_serialport->currentIndex();
    if( index > 0 && (com->connectState() < 2 || com->port() != comboBox_serialport->currentText()) )
        if( com->setRunMode(meta::connect) ) {
            com->setPort(comboBox_serialport->currentText()); //This should work for both user-specified and detected ports.
            com->start();
        }
}

void settingsdialog::disconnectSerial()
{
    com->closeport();
}

void settingsdialog::updateButtons()
{
    switch( com->connectState() ) {
    case meta::connected    : pushButton_connect->setEnabled(false);
                              pushButton_disconnect->setEnabled(true);
                              p_connectTries = -1;
                              break;
    case meta::connecting   : pushButton_connect->setEnabled(false);
                              pushButton_disconnect->setEnabled(false);
                              break;
    default                 : pushButton_connect->setEnabled(comboBox_serialport->currentIndex() > 0);
                              pushButton_disconnect->setEnabled(false);
                              if( p_connectTries > -1 ) {
                                  p_connectTries++;
                                  qDebug() << "[settingsdialog] connect try" << QString::number(p_connectTries);
                                  if( p_connectTries > 1 ) //two connect tries
                                      p_connectTries = -1;
                                  connectSerial();
                              }
                              break;
    }
    checkBox_autoconnect->setEnabled(comboBox_serialport->currentIndex() > 0);
}

void settingsdialog::reject()
{
    QDialog::reject();
}

void settingsdialog::accept()
{
    QSettings settings;
    settings.setValue("serial/port", !com->port().isEmpty() ? com->port() : comboBox_serialport->currentText());
    settings.setValue("serial/autoconnect",checkBox_autoconnect->isEnabled() ? checkBox_autoconnect->isChecked() : false);
    settings.setValue("plot/color1",channelColor[meta::channel1]);
    settings.setValue("plot/color2",channelColor[meta::channel2]);
    settings.setValue("plot/color3",channelColor[meta::channel3]);
    settings.setValue("plot/color4",channelColor[meta::channel4]);
    settings.setValue("plot/colorBackground",channelColor[meta::background]);
    settings.setValue("plot/colorGrid",channelColor[meta::grid]);
    QDialog::accept();
}

QColor settingsdialog::getChannelColor(meta::channel c)
{
    return channelColor[c];
}

void settingsdialog::getColorBackground()
{
    QColor newcolor = QColorDialog::getColor(channelColor[meta::background],this);
    if( newcolor != channelColor[meta::background] ) {
        channelColor[meta::background] = newcolor;
        updateColor(meta::background, newcolor);
        emit colorChanged(meta::background,newcolor);
    }
}

void settingsdialog::getColorGrid()
{
    QColor newcolor = QColorDialog::getColor(channelColor[meta::grid],this);
    if( newcolor != channelColor[meta::grid] ) {
        channelColor[meta::grid] = newcolor;
        updateColor(meta::grid, newcolor);
        emit colorChanged(meta::grid, newcolor);
    }
}

void settingsdialog::getColorChannel1()
{
    QColor newcolor = QColorDialog::getColor(channelColor[meta::channel1],this);
    if( newcolor != channelColor[meta::channel1] ) {
        channelColor[meta::channel1] = newcolor;
        updateColor(meta::channel1, newcolor);
        emit colorChanged(meta::channel1, newcolor);
    }
}

void settingsdialog::getColorChannel2()
{
    QColor newcolor = QColorDialog::getColor(channelColor[meta::channel2],this);
    if( newcolor != channelColor[meta::channel2] ) {
        channelColor[meta::channel2] = newcolor;
        updateColor(meta::channel2, newcolor);
        emit colorChanged(meta::channel2, newcolor);
    }
}

void settingsdialog::getColorChannel3()
{
    QColor newcolor = QColorDialog::getColor(channelColor[meta::channel3],this);
    if( newcolor != channelColor[meta::channel3] ) {
        channelColor[meta::channel3] = newcolor;
        updateColor(meta::channel3, newcolor);
        emit colorChanged(meta::channel3, newcolor);
    }
}

void settingsdialog::getColorChannel4()
{
    QColor newcolor = QColorDialog::getColor(channelColor[meta::channel4],this);
    if( newcolor != channelColor[meta::channel4] ) {
        channelColor[meta::channel4] = newcolor;
        updateColor(meta::channel4, newcolor);
        emit colorChanged(meta::channel4, newcolor);
    }
}

void settingsdialog::updateColor(meta::colorindex ci, QColor color)
{
    switch( ci ) {
    case meta::background : pushButton_colorBackground->setPalette(QPalette(color));
                            break;
    case meta::grid       : pushButton_colorGrid->setPalette(QPalette(color));
                            break;
    case meta::channel1   : pushButton_colorChannel1->setPalette(QPalette(color));
                            break;
    case meta::channel2   : pushButton_colorChannel2->setPalette(QPalette(color));
                            break;
    case meta::channel3   : pushButton_colorChannel3->setPalette(QPalette(color));
                            break;
    case meta::channel4   : pushButton_colorChannel4->setPalette(QPalette(color));
                            break;
    }
}
