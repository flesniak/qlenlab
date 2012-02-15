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

#ifndef BODEPLOT_H
#define BODEPLOT_H

#include <qwt/qwt_plot.h>
#include <QtGui/QDialog>

#include "meta.h"

class bodedata;
class communicator;
class QSpinBox;
class QProgressBar;
class QLabel;
class QRadioButton;
class QPushButton;
class QGroupBox;

class bodeplot : public QwtPlot
{
    Q_OBJECT
public:
    explicit bodeplot(communicator* com, QWidget *parent = 0);

private:
    communicator *p_com;
    bodedata* p_data;

public slots:
    bool exec();

    friend class bodedialog;
};

class bodedialog : public QDialog
{
    Q_OBJECT
public:
    explicit bodedialog(bodeplot *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

public slots:
    void updateStatus(bodestate state);
    void finished(bool stillmeasuring);

private:
    bodeplot *parent;

    QGroupBox *groupBox_parameters;
    QGroupBox *groupBox_inputButtons;
    QGroupBox *groupBox_outputButtons;
    QGroupBox *groupBox_bodeStatus;
    QSpinBox *spinBox_freqStart;
    QSpinBox *spinBox_freqEnd;
    QSpinBox *spinBox_decadePoints;
    QProgressBar *progressBar_progress;
    QLabel *label_inputAmplitude;
    QLabel *label_outputAmplitude;
    QLabel *label_amplification;
    QLabel *label_samplerate;
    QLabel *label_frequency;
    QRadioButton *radioButton_inputch1;
    QRadioButton *radioButton_inputch2;
    QRadioButton *radioButton_inputch3;
    QRadioButton *radioButton_inputch4;
    QRadioButton *radioButton_outputch1;
    QRadioButton *radioButton_outputch2;
    QRadioButton *radioButton_outputch3;
    QRadioButton *radioButton_outputch4;
    QPushButton *pushButton_start;
    QPushButton *pushButton_cancel;

private slots:
    void start();
    void cancel();
    void inputRadiosChanged();
    void outputRadiosChanged();
    meta::channel selectedInput();
    meta::channel selectedOutput();
};

#endif // BODEPLOT_H
