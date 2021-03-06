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

#ifndef DOCKWIDGETS_H
#define DOCKWIDGETS_H

#include <QtGui>

#include "meta.h"

class dockWidget_generator : public QDockWidget
{
    Q_OBJECT
public:
    dockWidget_generator(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    void restoreSettings();
    void saveSettings();

private:
    QSlider *slider_sinus;
    QSlider *slider_square;
    QSlider *slider_squareratio;
    QSpinBox *spinBox_sinus;
    QSpinBox *spinBox_square;
    QLabel *label_squareratio;
    QComboBox *comboBox_range_sinus;
    QComboBox *comboBox_range_square;
    QMutex freqChanging;

private slots:
    void freqSliderSinChanged(int value);
    void freqSliderSqrChanged(int value);
    void freqBoxSinChanged(int value);
    void freqBoxSqrChanged(int value);
    void freqRangeSinChanged(int value);
    void freqRangeSqrChanged(int value);
    void freqSliderSqrRatioChanged(int value);

    void submitSinusFreq();
    void submitSquareFreq();
    void submitSquareRatio();

signals:
    void freqSinChanged(unsigned short);
    void freqSqrChanged(unsigned short);
    void freqSqrRatioChanged(unsigned char);
};

class dockWidget_viewport: public QDockWidget
{
    Q_OBJECT
public:
    dockWidget_viewport(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    void restoreSettings();
    void saveSettings();
    void setMode(meta::plotmode mode);

private:
    QComboBox *comboBox_xaxis;
    QSpinBox *spinBox_xaxis;
    QDoubleSpinBox *spinBox_yaxis_lower;
    QDoubleSpinBox *spinBox_yaxis_upper;
    QCheckBox *checkBox_autoscale;
    QDoubleSpinBox *spinBox_autoscaleGrid;
    QSlider *slider_smoothFactor;
    QPushButton* pushButton_setZoom;

    int p_x_scope, p_x_fft;
    int p_y_min_scope;
    int p_y_max_scope, p_y_max_fft;
    bool p_autoscale_scope, p_autoscale_fft;
    double p_autoscale_grid_scope, p_autoscale_grid_fft;

    meta::plotmode p_mode;

    void backupVariables();

private slots:
    void updateViewportXValue(QString value);
    void submitViewportX();
    void submitViewportY();
    void submitSmoothFactor();
    void submitYAutoscale(bool on);
    void submitYAutoscaleGrid(double grid);

signals:
    void viewportXScopeChanged(const int);
    void viewportYScopeChanged(const double, const double);
    void autoscaleYScopeChanged(bool);
    void autoscaleYScopeGridChanged(double);

    void viewportXFftChanged(const int);
    void viewportYFftChanged(const double, const double);
    void autoscaleYFftChanged(bool);
    void autoscaleYFftGridChanged(double);

    void smoothFactorChanged(float);
};

class dockWidget_scope : public QDockWidget
{
    Q_OBJECT
public:
    dockWidget_scope(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    void restoreSettings();
    void saveSettings();

    QComboBox *comboBox_samplerate;
    QSpinBox *spinBox_samplerate;
    QCheckBox *checkBox_ch1active;
    QCheckBox *checkBox_ch2active;
    QCheckBox *checkBox_ch3active;
    QCheckBox *checkBox_ch4active;
    QDoubleSpinBox *spinBox_ch1offset;
    QDoubleSpinBox *spinBox_ch2offset;
    QDoubleSpinBox *spinBox_ch3offset;
    QDoubleSpinBox *spinBox_ch4offset;
    QCheckBox *checkBox_ch1offset;
    QCheckBox *checkBox_ch2offset;
    QCheckBox *checkBox_ch3offset;
    QCheckBox *checkBox_ch4offset;
    QCheckBox *checkBox_ch1invert;
    QCheckBox *checkBox_ch2invert;
    QCheckBox *checkBox_ch3invert;
    QCheckBox *checkBox_ch4invert;
    QComboBox *comboBox_range1;
    QComboBox *comboBox_range2;

public slots:
    void updateColor(meta::colorindex ci, QColor color);

private:
    QMutex comboMutex;

private slots:
    void submitSampleRate(int value = 0);
    void updateSampleRateSpinBox(QString value);
    bool checkSampleRate();

signals:
    void sampleRateChanged(unsigned int);
};

class dockWidget_trigger : public QDockWidget
{
    Q_OBJECT
public:
    dockWidget_trigger(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    void restoreSettings();
    void saveSettings();
    unsigned char getChannel() const;

private:
    QComboBox *comboBox_edge;
    QComboBox *comboBox_channel;
    QDoubleSpinBox *spinBox_triggervoltage;
    unsigned char p_enabled_channels;

public slots:
    void channel1changed(bool activated);
    void channel2changed(bool activated);
    void channel3changed(bool activated);
    void channel4changed(bool activated);

private slots:
    void comboBox_channel_changed(int index);
    void updateTriggerSpinBox(int index);
    void submitTriggerMode(double value);

signals:
    void triggerModeChanged(meta::triggermode mode, double value, unsigned char channel);
};

class storage;
class dockWidget_dataview : public QDockWidget
{
    Q_OBJECT
public:
    dockWidget_dataview(storage *datastorage, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    void restoreSettings();
    void saveSettings();

private:
    QSpinBox *spinBox_maxDatasets;
    storage *p_storage;
    QListView *listView_datasets;

private slots:
    void submitShowDataset(QModelIndex index);

signals:
    void showDataset(int);
    void maximumDatasetsChanged(int);
};

#endif // DOCKWIDGETS_H
