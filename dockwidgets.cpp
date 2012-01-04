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

#include "dockwidgets.h"

// ########## DOCKWIDGET GENERATOR #########

dockWidget_generator::dockWidget_generator(QWidget *parent, Qt::WindowFlags flags) : QDockWidget(parent,flags)
{
    setWindowTitle(tr("Frequenzgenerator"));
    setObjectName("dockWidget_generator");

    QWidget *widget_sinus = new QWidget;
    QWidget *widget_square = new QWidget;

    slider_sinus = new QSlider(Qt::Horizontal,widget_sinus);
    slider_sinus->setRange(1,1000);

    slider_square = new QSlider(Qt::Horizontal,widget_square);
    slider_square->setRange(1,1000);

    slider_squareratio = new QSlider(Qt::Horizontal,widget_square);
    slider_squareratio->setRange(1,99);
    slider_squareratio->setValue(50);

    spinBox_sinus = new QSpinBox(widget_sinus);
    spinBox_sinus->setRange(1,10000);
    spinBox_sinus->setSuffix("Hz");

    spinBox_square = new QSpinBox(widget_square);
    spinBox_square->setRange(1,10000);

    QLabel *label_square_ratio_dummy = new QLabel(tr("High-State"),widget_square);
    label_squareratio = new QLabel("50%",widget_square);

    comboBox_range_sinus = new QComboBox(widget_sinus);
    comboBox_range_sinus->addItems(QStringList() << tr("1-10000Hz exp")
                                                 << tr("1-100Hz")
                                                 << tr("100-1000Hz")
                                                 << tr("1000-10000Hz"));

    comboBox_range_square = new QComboBox(widget_square);
    comboBox_range_square->addItems(QStringList() << tr("1-10000Hz exp")
                                                  << tr("1-100Hz")
                                                  << tr("100-1000Hz")
                                                  << tr("1000-10000Hz"));

    QHBoxLayout *layout_sinus_h = new QHBoxLayout;
    layout_sinus_h->addWidget(comboBox_range_sinus);
    layout_sinus_h->addWidget(spinBox_sinus);

    QVBoxLayout *layout_sinus = new QVBoxLayout(widget_sinus);
    layout_sinus->addWidget(slider_sinus);
    layout_sinus->addLayout(layout_sinus_h);

    QHBoxLayout *layout_square_1 = new QHBoxLayout;
    layout_square_1->addWidget(comboBox_range_square);
    layout_square_1->addWidget(spinBox_square);

    QHBoxLayout *layout_square_2 = new QHBoxLayout;
    layout_square_2->addWidget(label_square_ratio_dummy);
    layout_square_2->addWidget(slider_squareratio);
    layout_square_2->addWidget(label_squareratio);

    QVBoxLayout *layout_square = new QVBoxLayout(widget_square);
    layout_square->addWidget(slider_square);
    layout_square->addLayout(layout_square_1);
    layout_square->addLayout(layout_square_2);

    QTabWidget *tabwidget = new QTabWidget(this);
    tabwidget->addTab(widget_sinus,tr("Sinus"));
    tabwidget->addTab(widget_square,tr("Rechteck"));

    setWidget(tabwidget);

    connect(slider_sinus,SIGNAL(valueChanged(int)),SLOT(freqSliderSinChanged(int)));
    connect(slider_square,SIGNAL(valueChanged(int)),SLOT(freqSliderSqrChanged(int)));
    connect(spinBox_sinus,SIGNAL(valueChanged(int)),SLOT(freqBoxSinChanged(int)));
    connect(spinBox_square,SIGNAL(valueChanged(int)),SLOT(freqBoxSqrChanged(int)));
    connect(comboBox_range_sinus,SIGNAL(currentIndexChanged(int)),SLOT(freqRangeSinChanged(int)));
    connect(comboBox_range_square,SIGNAL(currentIndexChanged(int)),SLOT(freqRangeSqrChanged(int)));
    connect(slider_squareratio,SIGNAL(valueChanged(int)),SLOT(freqSliderSqrRatioChanged(int)));

    connect(slider_sinus,SIGNAL(sliderReleased()),SLOT(submitSinusFreq()));
    connect(slider_square,SIGNAL(sliderReleased()),SLOT(submitSquareFreq()));
    connect(slider_squareratio,SIGNAL(sliderReleased()),SLOT(submitSquareRatio()));

};

void dockWidget_generator::freqSliderSinChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( comboBox_range_sinus->currentIndex() == 0 )
            spinBox_sinus->setValue(round(1.0*value*value/100+1));
        else
            spinBox_sinus->setValue(value);
        freqChanging.unlock();
    }
}

void dockWidget_generator::freqSliderSqrChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( comboBox_range_square->currentIndex() == 0 )
            spinBox_square->setValue(round(1.0*value*value/100+1));
        else
            spinBox_square->setValue(value);
        freqChanging.unlock();
    }
}

void dockWidget_generator::freqBoxSinChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( comboBox_range_sinus->currentIndex() == 0 )
            slider_sinus->setValue(round(10.0*sqrt(value-(99.0/100))));
        else
            slider_sinus->setValue(value);
        submitSinusFreq();
        freqChanging.unlock();
    }
}

void dockWidget_generator::freqBoxSqrChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( comboBox_range_square->currentIndex() == 0 )
            slider_square->setValue(round(10.0*sqrt(value-(99.0/100))));
        else
            slider_square->setValue(value);
        submitSquareFreq();
        freqChanging.unlock();
    }
}

void dockWidget_generator::freqRangeSinChanged(int index)
{
    freqChanging.lock();
    switch( index ) {
    case 0 : spinBox_sinus->setRange(1,10000);
             slider_sinus->setRange(1,1000);
             break;
    case 1 : spinBox_sinus->setRange(1,100);
             slider_sinus->setRange(1,100);
             break;
    case 2 : spinBox_sinus->setRange(100,1000);
             slider_sinus->setRange(100,1000);
             break;
    case 3 : spinBox_sinus->setRange(1000,10000);
             slider_sinus->setRange(1000,10000);
             break;
    default: break;
    }
    freqChanging.unlock();
    freqBoxSinChanged(spinBox_sinus->value());
}

void dockWidget_generator::freqRangeSqrChanged(int index)
{
    freqChanging.lock();
    switch( index ) {
    case 0 : spinBox_square->setRange(1,10000);
             slider_square->setRange(1,1000);
             break;
    case 1 : spinBox_square->setRange(1,100);
             slider_square->setRange(1,100);
             break;
    case 2 : spinBox_square->setRange(100,1000);
             slider_square->setRange(100,1000);
             break;
    case 3 : spinBox_square->setRange(1000,10000);
             slider_square->setRange(1000,10000);
             break;
    default: break;
    }
    freqChanging.unlock();
    freqBoxSqrChanged(spinBox_square->value());
}

void dockWidget_generator::freqSliderSqrRatioChanged(int value)
{
    label_squareratio->setText(QString::number(value)+"%");
}

void dockWidget_generator::submitSinusFreq()
{
    emit freqSinChanged(spinBox_sinus->value());
}

void dockWidget_generator::submitSquareFreq()
{
    emit freqSqrChanged(spinBox_square->value());
}

void dockWidget_generator::submitSquareRatio()
{
    emit freqSqrRatioChanged(slider_squareratio->value());
}

// ########## DOCKWIDGET VIEWPORT #########

dockWidget_viewport::dockWidget_viewport(QWidget *parent, Qt::WindowFlags flags) : QDockWidget(parent,flags)
{
    setWindowTitle(tr("Anzeigeeinstellungen"));
    setObjectName("dockWidget_viewport");

    QWidget *widget = new QWidget(this);

    QGroupBox *groupBox_xaxis = new QGroupBox(tr("X-Achse"));
    QGroupBox *groupBox_yaxis = new QGroupBox(tr("Y-Achse"));

    comboBox_xaxis = new QComboBox(groupBox_xaxis);
    comboBox_xaxis->addItems(QStringList() << "5" << "10" << "20" << "50" << "100" << "200" << "500" << "1000" << "2000");

    spinBox_xaxis = new QSpinBox(groupBox_xaxis);
    spinBox_xaxis->setRange(1,10000);
    spinBox_xaxis->setValue(20);
    spinBox_xaxis->setSuffix("ms");

    QHBoxLayout *layout_xaxis = new QHBoxLayout(groupBox_xaxis);
    layout_xaxis->addWidget(comboBox_xaxis);
    layout_xaxis->addWidget(spinBox_xaxis);

    spinBox_yaxis_lower = new QDoubleSpinBox(groupBox_yaxis);
    spinBox_yaxis_lower->setRange(-40,40);
    spinBox_yaxis_lower->setValue(-5);
    spinBox_yaxis_lower->setSuffix("V");
    spinBox_yaxis_upper = new QDoubleSpinBox(groupBox_yaxis);
    spinBox_yaxis_upper->setRange(-40,40);
    spinBox_yaxis_upper->setValue(5);
    spinBox_yaxis_upper->setSuffix("V");

    QLabel *label_yaxis_dummy = new QLabel("-");
    label_yaxis_dummy->setAlignment(Qt::AlignCenter);

    checkBox_autoscale = new QCheckBox(tr("Autoscale"),groupBox_yaxis);
    checkBox_autoscale->setEnabled(false);

    QHBoxLayout *layout_yaxis_1 = new QHBoxLayout;
    layout_yaxis_1->addWidget(spinBox_yaxis_lower);
    layout_yaxis_1->addWidget(label_yaxis_dummy);
    layout_yaxis_1->addWidget(spinBox_yaxis_upper);

    QVBoxLayout *layout_yaxis = new QVBoxLayout(groupBox_yaxis);
    layout_yaxis->addLayout(layout_yaxis_1);
    layout_yaxis->addWidget(checkBox_autoscale);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(groupBox_xaxis);
    layout->addWidget(groupBox_yaxis);

    connect(comboBox_xaxis,SIGNAL(currentIndexChanged(QString)),SLOT(updateViewportXValue(QString)));
    connect(spinBox_xaxis,SIGNAL(valueChanged(int)),SLOT(submitViewportX()));
    connect(spinBox_yaxis_lower,SIGNAL(valueChanged(double)),SLOT(submitViewportY()));
    connect(spinBox_yaxis_upper,SIGNAL(valueChanged(double)),SLOT(submitViewportY()));

    setWidget(widget);
}

void dockWidget_viewport::updateViewportXValue(QString value)
{
    spinBox_xaxis->setValue(value.toInt());
}

void dockWidget_viewport::submitViewportX()
{
    emit viewportXChanged(spinBox_xaxis->value());
}

void dockWidget_viewport::submitViewportY()
{
    double upper = spinBox_yaxis_upper->value();
    spinBox_yaxis_lower->setMaximum(upper);
    emit viewportYChanged(spinBox_yaxis_lower->value(),upper);
}

// ########## DOCKWIDGET SCOPE #########

dockWidget_scope::dockWidget_scope(QWidget *parent, Qt::WindowFlags flags) : QDockWidget(parent,flags)
{
    setWindowTitle(tr("Oszilloskop"));
    setObjectName("dockWidget_scope");

    QWidget *widget = new QWidget(this);

    QLabel *label_samplerate_dummy = new QLabel(tr("Samplerate"),widget);
    QLabel *label_manoffset_dummy = new QLabel(tr("Offset"));
    QLabel *label_offset_dummy = new QLabel("  ~",widget);
    label_offset_dummy->setAlignment(Qt::AlignLeft);
    QLabel *label_invert_dummy = new QLabel(tr("Inv"),widget);
    QLabel *label_range1_dummy = new QLabel(tr("Range 1/2"),widget);
    QLabel *label_range2_dummy = new QLabel(tr("Range 3/4"),widget);

    comboBox_samplerate = new QComboBox(widget);
    comboBox_samplerate->addItems(QStringList() << "400"
                                  << "200"
                                  << "125"
                                  << "100"
                                  << "50"
                                  << "10"
                                  << "1");
    spinBox_samplerate = new QSpinBox(widget);
    spinBox_samplerate->setRange(1,400);
    spinBox_samplerate->setSuffix("kHz");

    checkBox_ch1active = new QCheckBox(tr("Kanal 1"),widget);
    checkBox_ch2active = new QCheckBox(tr("Kanal 2"),widget);
    checkBox_ch3active = new QCheckBox(tr("Kanal 3"),widget);
    checkBox_ch4active = new QCheckBox(tr("Kanal 4"),widget);

    spinBox_ch1offset = new QDoubleSpinBox(widget);
    spinBox_ch1offset->setRange(-40,40);
    spinBox_ch1offset->setSuffix("V");
    spinBox_ch2offset = new QDoubleSpinBox(widget);
    spinBox_ch2offset->setRange(-40,40);
    spinBox_ch2offset->setSuffix("V");
    spinBox_ch3offset = new QDoubleSpinBox(widget);
    spinBox_ch3offset->setRange(-40,40);
    spinBox_ch3offset->setSuffix("V");
    spinBox_ch4offset = new QDoubleSpinBox(widget);
    spinBox_ch4offset->setRange(-40,40);
    spinBox_ch4offset->setSuffix("V");

    checkBox_ch1offset = new QCheckBox(widget);
    checkBox_ch2offset = new QCheckBox(widget);
    checkBox_ch3offset = new QCheckBox(widget);
    checkBox_ch4offset = new QCheckBox(widget);

    checkBox_ch1invert = new QCheckBox(widget);
    checkBox_ch2invert = new QCheckBox(widget);
    checkBox_ch3invert = new QCheckBox(widget);
    checkBox_ch4invert = new QCheckBox(widget);

    comboBox_range1 = new QComboBox(this);
    comboBox_range1->addItems(QStringList() << "0.5x (1.6V)"
                                            << "1x (3.3V)"
                                            << "2x (6.6V)"
                                            << "10x (33V)");
    comboBox_range1->setCurrentIndex(3);
    comboBox_range2 = new QComboBox(this);
    comboBox_range2->addItems(QStringList() << "0.5x (1.6V)"
                                            << "1x (3.3V)"
                                            << "2x (6.6V)"
                                            << "10x (33V)");
    comboBox_range2->setCurrentIndex(3);

    QHBoxLayout *layout_samplerate = new QHBoxLayout;
    layout_samplerate->addWidget(label_samplerate_dummy);
    layout_samplerate->addWidget(comboBox_samplerate);
    layout_samplerate->addWidget(spinBox_samplerate);

    QGridLayout *layout_range = new QGridLayout;
    layout_range->addWidget(label_range1_dummy,0,0);
    layout_range->addWidget(label_range2_dummy,0,1);
    layout_range->addWidget(comboBox_range1,1,0);
    layout_range->addWidget(comboBox_range2,1,1);

    QGridLayout *layout = new QGridLayout(widget);
    layout->addLayout(layout_samplerate,0,0,1,4);
    layout->addWidget(label_manoffset_dummy,1,1);
    layout->addWidget(label_offset_dummy,1,2);
    layout->addWidget(label_invert_dummy,1,3);
    layout->addWidget(checkBox_ch1active,2,0);
    layout->addWidget(checkBox_ch2active,3,0);
    layout->addWidget(checkBox_ch3active,4,0);
    layout->addWidget(checkBox_ch4active,5,0);
    layout->addWidget(spinBox_ch1offset,2,1);
    layout->addWidget(spinBox_ch2offset,3,1);
    layout->addWidget(spinBox_ch3offset,4,1);
    layout->addWidget(spinBox_ch4offset,5,1);
    layout->addWidget(checkBox_ch1offset,2,2);
    layout->addWidget(checkBox_ch2offset,3,2);
    layout->addWidget(checkBox_ch3offset,4,2);
    layout->addWidget(checkBox_ch4offset,5,2);
    layout->addWidget(checkBox_ch1invert,2,3);
    layout->addWidget(checkBox_ch2invert,3,3);
    layout->addWidget(checkBox_ch3invert,4,3);
    layout->addWidget(checkBox_ch4invert,5,3);
    layout->addLayout(layout_range,6,0,6,4);

    connect(comboBox_samplerate,SIGNAL(currentIndexChanged(QString)),SLOT(updateSampleRateSpinBox(QString)));
    connect(spinBox_samplerate,SIGNAL(valueChanged(int)),SLOT(submitSampleRate(int)));

    setWidget(widget);

    comboBox_samplerate->setCurrentIndex(3);
}

void dockWidget_scope::updateSampleRateSpinBox(QString value)
{
    if( !value.isEmpty() )
        spinBox_samplerate->setValue(value.toInt());
}

void dockWidget_scope::submitSampleRate(int value)
{
    unsigned char actives = checkBox_ch1active->isChecked() ? 1 : 0 + checkBox_ch2active->isChecked() ? 1 : 0 + checkBox_ch3active->isChecked() ? 1 : 0 + checkBox_ch4active->isChecked() ? 1 : 0;
    switch( actives ) {
    case 4 : if( spinBox_samplerate->value() > 100 )
                 spinBox_samplerate->setValue(100);
             break;
    case 3 : if( spinBox_samplerate->value() > 125 )
                 spinBox_samplerate->setValue(125);
             break;
    case 2 : if( spinBox_samplerate->value() > 200 )
                 spinBox_samplerate->setValue(200);
             break;
    default : break;
    }
    comboBox_samplerate->setCurrentIndex(comboBox_samplerate->findText(QString::number(value)));
    emit sampleRateChanged(value);
}
