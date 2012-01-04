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

#ifndef DOCKWIDGETS_H
#define DOCKWIDGETS_H

#include <QtGui>

class dockWidget_generator : public QDockWidget
{
    Q_OBJECT
public:
    dockWidget_generator(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    void restoreSettings();
    void saveSettings();

    QSlider *slider_sinus;
    QSlider *slider_square;
    QSlider *slider_squareratio;
    QSpinBox *spinBox_sinus;
    QSpinBox *spinBox_square;
    QLabel *label_squareratio;
    QComboBox *comboBox_range_sinus;
    QComboBox *comboBox_range_square;

private:
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

    QComboBox *comboBox_xaxis;
    QSpinBox *spinBox_xaxis;
    QDoubleSpinBox *spinBox_yaxis_lower;
    QDoubleSpinBox *spinBox_yaxis_upper;
    QCheckBox *checkBox_autoscale;

private slots:
    void updateViewportXValue(QString value);
    void submitViewportX();
    void submitViewportY();

signals:
    void viewportXChanged(const int);
    void viewportYChanged(const double, const double);
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

private:
    QMutex comboMutex;

private slots:
    void submitSampleRate(int value);
    void updateSampleRateSpinBox(QString value);

signals:
    void sampleRateChanged(unsigned int);
};

class dockWidget_trigger : public QDockWidget
{

};

#endif // DOCKWIDGETS_H
