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

#ifndef QLENLAB_H
#define QLENLAB_H

#include <QMainWindow>
#include <QLabel>
#include <QTabWidget>
#include <QString>
#include <QMutex>

#include "meta.h"

class settingsdialog;
class plot;
class communicator;
class signaldata;

namespace Ui {
    class QLenLab;
}

class QLenLab : public QMainWindow
{
    Q_OBJECT

public:
    explicit QLenLab(QWidget *parent = 0);
    ~QLenLab();

public slots:
    void setConnectionStatus(bool connected);

protected:
    void closeEvent(QCloseEvent *);

private:
    void restoreSettings();
    void initCom();
    settingsdialog *settingsdlg;
    QLabel *label_connectionstatus;
    QTabWidget *tabWidget;
    Ui::QLenLab *ui;
    plot *plotter;
    communicator* com;
    signaldata* data;
    QMutex freqChanging;

private slots:
    void viewportXChanged(QString);
    void viewportYChanged();
    void freqSliderSinChanged(int);
    void freqBoxSinChanged(int);
    void freqRangeSinChanged(int);
    void submitSinusFreq();
    void freqSliderSqrChanged(int);
    void freqBoxSqrChanged(int);
    void freqRangeSqrChanged(int);
    void freqSliderSqrRatioChanged(int);
    void submitSquareFreq();
    void submitSquareRatio();
    void submitSampleRate(int);
    void start();
    void stop();
    void showDebug();
    void showSettings();
    void about();
    void quit();
};

#endif // QLENLAB_H
