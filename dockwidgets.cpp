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

#include "dockwidgets.h"
#include "storage.h"

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
    spinBox_square->setSuffix("Hz");

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
}

void dockWidget_generator::restoreSettings()
{
    QSettings settings;
    spinBox_sinus->setValue(settings.value("generator/sinus",1).toInt());
    spinBox_square->setValue(settings.value("generator/square",1).toInt());
    slider_squareratio->setValue(settings.value("generator/squareratio",50).toInt());
    submitSinusFreq();
    submitSquareFreq();
    submitSquareRatio();
}

void dockWidget_generator::saveSettings()
{
    QSettings settings;
    settings.setValue("generator/sinus",spinBox_sinus->value());
    settings.setValue("generator/square",spinBox_square->value());
    settings.setValue("generator/squareratio",slider_squareratio->value());
}

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

dockWidget_viewport::dockWidget_viewport(QWidget *parent, Qt::WindowFlags flags) : QDockWidget(parent,flags), p_mode(meta::scope)
{
    setWindowTitle(tr("Anzeigeeinstellungen"));
    setObjectName("dockWidget_viewport");

    QWidget *widget = new QWidget(this);

    QGroupBox *groupBox_xaxis = new QGroupBox(tr("X-Achse"));
    QGroupBox *groupBox_yaxis = new QGroupBox(tr("Y-Achse"));

    p_x_scope = 200;    p_x_fft = 1000;
    p_y_min_scope = -10;
    p_y_max_scope = 10;  p_y_max_fft = 20;
    p_autoscale_scope = 0;  p_autoscale_fft = 0;
    p_autoscale_grid_scope = 0; p_autoscale_grid_fft = 0;

    //these elements will be configured by setMode
    comboBox_xaxis = new QComboBox(groupBox_xaxis);
    spinBox_xaxis = new QSpinBox(groupBox_xaxis);
    spinBox_yaxis_lower = new QDoubleSpinBox(groupBox_yaxis);
    spinBox_yaxis_upper = new QDoubleSpinBox(groupBox_yaxis);

    QLabel *label_yaxis_dummy = new QLabel("-",groupBox_yaxis);
    label_yaxis_dummy->setAlignment(Qt::AlignCenter);

    pushButton_setZoom = new QPushButton(QIcon::fromTheme("zoom-fit-best"),QString(),this);

    checkBox_autoscale = new QCheckBox(tr("Autoscale"),groupBox_yaxis);
    spinBox_autoscaleGrid = new QDoubleSpinBox(groupBox_yaxis);
    QLabel *label_autoscaleGrid = new QLabel(tr("Intervall"),groupBox_yaxis);
    label_autoscaleGrid->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    spinBox_autoscaleGrid->setRange(0.0,10.0);
    spinBox_autoscaleGrid->setSingleStep(0.5);
    spinBox_autoscaleGrid->setEnabled(false);

    QLabel *label_smoothFactor_dummy = new QLabel("Glättung",widget);
    slider_smoothFactor = new QSlider(Qt::Horizontal,widget);
    slider_smoothFactor->setRange(0,100);

    QHBoxLayout *layout_xaxis = new QHBoxLayout(groupBox_xaxis);
    layout_xaxis->addWidget(comboBox_xaxis);
    layout_xaxis->addWidget(spinBox_xaxis);

    QHBoxLayout *layout_yaxis_1 = new QHBoxLayout;
    layout_yaxis_1->addWidget(spinBox_yaxis_lower);
    layout_yaxis_1->addWidget(label_yaxis_dummy);
    layout_yaxis_1->addWidget(spinBox_yaxis_upper);
    layout_yaxis_1->addWidget(pushButton_setZoom);

    QHBoxLayout *layout_yaxis_2 = new QHBoxLayout;
    layout_yaxis_2->addWidget(checkBox_autoscale);
    layout_yaxis_2->addWidget(label_autoscaleGrid);
    layout_yaxis_2->addWidget(spinBox_autoscaleGrid);

    QVBoxLayout *layout_yaxis = new QVBoxLayout(groupBox_yaxis);
    layout_yaxis->addLayout(layout_yaxis_1);
    layout_yaxis->addLayout(layout_yaxis_2);

    QHBoxLayout *layout_smooth = new QHBoxLayout;
    layout_smooth->addWidget(label_smoothFactor_dummy);
    layout_smooth->addWidget(slider_smoothFactor);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(groupBox_xaxis);
    layout->addWidget(groupBox_yaxis);
    layout->addLayout(layout_smooth);

    connect(comboBox_xaxis,SIGNAL(currentIndexChanged(QString)),SLOT(updateViewportXValue(QString)));
    connect(spinBox_xaxis,SIGNAL(valueChanged(int)),SLOT(submitViewportX()));
    connect(spinBox_yaxis_lower,SIGNAL(valueChanged(double)),SLOT(submitViewportY()));
    connect(spinBox_yaxis_upper,SIGNAL(valueChanged(double)),SLOT(submitViewportY()));
    connect(checkBox_autoscale,SIGNAL(toggled(bool)),SLOT(submitYAutoscale(bool)));
    connect(checkBox_autoscale,SIGNAL(toggled(bool)),spinBox_autoscaleGrid,SLOT(setEnabled(bool)));
    connect(spinBox_autoscaleGrid,SIGNAL(valueChanged(double)),SLOT(submitYAutoscaleGrid(double)));
    connect(slider_smoothFactor,SIGNAL(valueChanged(int)),SLOT(submitSmoothFactor()));
    connect(pushButton_setZoom,SIGNAL(clicked()),SLOT(submitViewportX()));
    connect(pushButton_setZoom,SIGNAL(clicked()),SLOT(submitViewportY()));

    setWidget(widget);
    setMode(meta::scope);
}

void dockWidget_viewport::restoreSettings()
{
    QSettings settings;
    p_x_scope = settings.value("viewport/xaxis",200).toInt();
    p_y_min_scope = settings.value("viewport/yaxis_lower").toDouble();
    p_y_max_scope = settings.value("viewport/yaxis_upper").toDouble();
    if( p_y_min_scope >= p_y_max_scope) {
        p_y_max_scope = 10;
        p_y_min_scope = -10;
    }
    p_autoscale_scope = settings.value("viewport/yaxis_autoscale",false).toBool();
    p_autoscale_grid_scope = settings.value("viewport/yaxis_autoscalegrid",2.0).toDouble();

    p_x_fft = settings.value("viewport/xaxis_fft",1000).toInt();
    p_y_max_fft = settings.value("viewport/yaxis_upper_fft").toDouble();
    if( p_y_max_fft <=  0)
        p_y_max_fft = 10;
    p_autoscale_fft = settings.value("viewport/yaxis_autoscale_fft",false).toBool();
    p_autoscale_grid_fft = settings.value("viewport/yaxis_autoscalegrid_fft",2.0).toDouble();

    setMode(meta::scope);

    slider_smoothFactor->setValue(settings.value("viewport/smoothfactor",0).toInt());
}

void dockWidget_viewport::saveSettings()
{
    backupVariables();
    QSettings settings;
    settings.setValue("viewport/xaxis",p_x_scope);
    settings.setValue("viewport/yaxis_lower",p_y_min_scope);
    settings.setValue("viewport/yaxis_upper",p_y_max_scope);
    settings.setValue("viewport/yaxis_autoscale",p_autoscale_scope);
    settings.setValue("viewport/yaxis_autoscalegrid",p_autoscale_grid_scope);

    settings.setValue("viewport/xaxis_fft",p_x_fft);
    settings.setValue("viewport/yaxis_upper_fft",p_y_max_fft);
    settings.setValue("viewport/yaxis_autoscale_fft",p_autoscale_fft);
    settings.setValue("viewport/yaxis_autoscalegrid_fft",p_autoscale_grid_fft);

    settings.setValue("viewport/smoothfactor",slider_smoothFactor->value());
}

void dockWidget_viewport::backupVariables()
{
    switch(p_mode) {
    case meta::scope :
        p_x_scope = spinBox_xaxis->value();
        p_y_min_scope = spinBox_yaxis_lower->value();
        p_y_max_scope = spinBox_yaxis_upper->value();
        p_autoscale_scope = checkBox_autoscale->isChecked();
        p_autoscale_grid_scope = spinBox_autoscaleGrid->value();
        break;
    case meta::fft :
        p_x_fft = spinBox_xaxis->value();
        p_y_max_fft = spinBox_yaxis_upper->value();
        p_autoscale_fft = checkBox_autoscale->isChecked();
        p_autoscale_grid_fft = spinBox_autoscaleGrid->value();
        break;
    }
}

void dockWidget_viewport::setMode(meta::plotmode mode)
{
    if( mode != p_mode )
        backupVariables();
    switch(mode) {
    case meta::scope :
        p_mode = mode;
        comboBox_xaxis->clear();
        comboBox_xaxis->blockSignals(true);
        comboBox_xaxis->addItems(QStringList() << "5" << "10" << "20" << "50" << "100" << "200" << "500" << "1000" << "2000");
        comboBox_xaxis->blockSignals(false);
        spinBox_xaxis->setRange(1,10000);
        spinBox_xaxis->setSuffix("ms");
        spinBox_yaxis_lower->setEnabled(true);
        spinBox_yaxis_lower->setRange(-40,40);
        spinBox_yaxis_lower->setSuffix("V");
        spinBox_yaxis_upper->setRange(-40,40);
        spinBox_yaxis_upper->setSuffix("V");

        spinBox_xaxis->setValue(p_x_scope);
        spinBox_yaxis_upper->setValue(p_y_max_scope);
        spinBox_yaxis_lower->setValue(p_y_min_scope);
        checkBox_autoscale->setChecked(p_autoscale_scope);
        spinBox_autoscaleGrid->setValue(p_autoscale_grid_scope);
        break;
    case meta::fft :
        p_mode = mode;
        comboBox_xaxis->clear();
        comboBox_xaxis->blockSignals(true);
        comboBox_xaxis->addItems(QStringList() << "10" << "50" << "100" << "500" << "1000" << "5000" << "10000" << "50000");
        comboBox_xaxis->blockSignals(false);
        spinBox_xaxis->setRange(1,100000);
        spinBox_xaxis->setSuffix("Hz");
        spinBox_yaxis_lower->setEnabled(false);
        spinBox_yaxis_upper->setRange(0.01,40);
        spinBox_yaxis_upper->setSuffix("V");

        spinBox_xaxis->setValue(p_x_fft);
        spinBox_yaxis_upper->setValue(p_y_max_fft);
        spinBox_yaxis_lower->setValue(0);
        checkBox_autoscale->setChecked(p_autoscale_fft);
        spinBox_autoscaleGrid->setValue(p_autoscale_grid_fft);
        break;
    }
    submitViewportX();
    submitViewportY();
}

void dockWidget_viewport::updateViewportXValue(QString value)
{
    if( !value.isEmpty() )
        spinBox_xaxis->setValue(value.toInt());
}

void dockWidget_viewport::submitSmoothFactor()
{
    emit smoothFactorChanged(sqrt(slider_smoothFactor->value())/10);
}

void dockWidget_viewport::submitYAutoscale(bool on)
{
    if( on ) {
        switch(p_mode) {
        case meta::scope :
            emit autoscaleYScopeChanged(on);
            break;
        case meta::fft :
            emit autoscaleYFftChanged(on);
            break;
        }
    } else {
        submitViewportX();
        submitViewportY();
    }
}

void dockWidget_viewport::submitViewportX()
{
    comboBox_xaxis->setCurrentIndex(comboBox_xaxis->findText(QString::number(spinBox_xaxis->value())));
    switch(p_mode) {
    case meta::scope :
        emit viewportXScopeChanged(spinBox_xaxis->value());
        break;
    case meta::fft :
        emit viewportXFftChanged(spinBox_xaxis->value());
        break;
    }
}

void dockWidget_viewport::submitViewportY()
{
    checkBox_autoscale->setChecked(false);
    const double upper = spinBox_yaxis_upper->value();
    spinBox_yaxis_lower->setMaximum(upper-0.01);
    switch(p_mode) {
    case meta::scope :
        emit viewportYScopeChanged(spinBox_yaxis_lower->value(),upper);
        break;
    case meta::fft :
        emit viewportYFftChanged(0,upper);
        break;
    }
}

void dockWidget_viewport::submitYAutoscaleGrid(double grid)
{
    switch(p_mode) {
    case meta::scope :
        emit autoscaleYScopeGridChanged(grid);
        break;
    case meta::fft :
        emit autoscaleYFftGridChanged(grid);
        break;
    }
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
    comboBox_samplerate->setCurrentIndex(3);
    spinBox_samplerate = new QSpinBox(widget);
    spinBox_samplerate->setRange(1,400);
    spinBox_samplerate->setValue(100);
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
    checkBox_ch1offset->setChecked(true);
    checkBox_ch2offset->setChecked(true);
    checkBox_ch3offset->setChecked(true);
    checkBox_ch4offset->setChecked(true);

    checkBox_ch1invert = new QCheckBox(widget);
    checkBox_ch2invert = new QCheckBox(widget);
    checkBox_ch3invert = new QCheckBox(widget);
    checkBox_ch4invert = new QCheckBox(widget);

    comboBox_range1 = new QComboBox(this);
    comboBox_range1->addItems(QStringList() << "0.5x (1.6V)"
                                            << "1x (3.3V)"
                                            << "2x (6.6V)"
                                            << "10x (33V)");
    comboBox_range1->setCurrentIndex(-1);
    comboBox_range2 = new QComboBox(this);
    comboBox_range2->addItems(QStringList() << "0.5x (1.6V)"
                                            << "1x (3.3V)"
                                            << "2x (6.6V)"
                                            << "10x (33V)");
    comboBox_range2->setCurrentIndex(-1);

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
    connect(checkBox_ch1active,SIGNAL(toggled(bool)),SLOT(checkSampleRate()));
    connect(checkBox_ch2active,SIGNAL(toggled(bool)),SLOT(checkSampleRate()));
    connect(checkBox_ch3active,SIGNAL(toggled(bool)),SLOT(checkSampleRate()));
    connect(checkBox_ch4active,SIGNAL(toggled(bool)),SLOT(checkSampleRate()));

    setWidget(widget);
}

void dockWidget_scope::restoreSettings()
{
    QSettings settings;
    checkBox_ch1active->setChecked(settings.value("scope/ch1active",true).toBool());
    checkBox_ch2active->setChecked(settings.value("scope/ch2active",false).toBool());
    checkBox_ch3active->setChecked(settings.value("scope/ch3active",false).toBool());
    checkBox_ch4active->setChecked(settings.value("scope/ch4active",false).toBool());
    checkBox_ch1offset->setChecked(settings.value("scope/ch1alternate",false).toBool());
    checkBox_ch2offset->setChecked(settings.value("scope/ch2alternate",false).toBool());
    checkBox_ch3offset->setChecked(settings.value("scope/ch3alternate",false).toBool());
    checkBox_ch4offset->setChecked(settings.value("scope/ch4alternate",false).toBool());
    checkBox_ch1invert->setChecked(settings.value("scope/ch1invert",false).toBool());
    checkBox_ch2invert->setChecked(settings.value("scope/ch2invert",false).toBool());
    checkBox_ch3invert->setChecked(settings.value("scope/ch3invert",false).toBool());
    checkBox_ch4invert->setChecked(settings.value("scope/ch4invert",false).toBool());
    spinBox_ch1offset->setValue(settings.value("scope/ch1offset",0.0).toDouble());
    spinBox_ch2offset->setValue(settings.value("scope/ch2offset",0.0).toDouble());
    spinBox_ch3offset->setValue(settings.value("scope/ch3offset",0.0).toDouble());
    spinBox_ch4offset->setValue(settings.value("scope/ch4offset",0.0).toDouble());
    spinBox_samplerate->setValue(settings.value("scope/samplerate",100).toInt());
    comboBox_range1->setCurrentIndex(settings.value("scope/range1_index",3).toInt());
    comboBox_range2->setCurrentIndex(settings.value("scope/range2_index",3).toInt());
}

void dockWidget_scope::saveSettings()
{
    QSettings settings;
    settings.setValue("scope/ch1active",checkBox_ch1active->isChecked());
    settings.setValue("scope/ch1alternate",checkBox_ch1offset->isChecked());
    settings.setValue("scope/ch1invert",checkBox_ch1invert->isChecked());
    settings.setValue("scope/ch1offset",spinBox_ch1offset->value());
    settings.setValue("scope/ch2active",checkBox_ch2active->isChecked());
    settings.setValue("scope/ch2alternate",checkBox_ch2offset->isChecked());
    settings.setValue("scope/ch2invert",checkBox_ch2invert->isChecked());
    settings.setValue("scope/ch2offset",spinBox_ch2offset->value());
    settings.setValue("scope/ch3active",checkBox_ch3active->isChecked());
    settings.setValue("scope/ch3alternate",checkBox_ch3offset->isChecked());
    settings.setValue("scope/ch3invert",checkBox_ch3invert->isChecked());
    settings.setValue("scope/ch3offset",spinBox_ch3offset->value());
    settings.setValue("scope/ch4active",checkBox_ch4active->isChecked());
    settings.setValue("scope/ch4alternate",checkBox_ch4offset->isChecked());
    settings.setValue("scope/ch4invert",checkBox_ch4invert->isChecked());
    settings.setValue("scope/ch4offset",spinBox_ch4offset->value());
    settings.setValue("scope/samplerate",spinBox_samplerate->value());
    settings.setValue("scope/range1_index",comboBox_range1->currentIndex());
    settings.setValue("scope/range2_index",comboBox_range2->currentIndex());
}

void dockWidget_scope::updateSampleRateSpinBox(QString value)
{
    if( !value.isEmpty() )
        spinBox_samplerate->setValue(value.toInt());
}

bool dockWidget_scope::checkSampleRate()
{
    unsigned char actives = (checkBox_ch1active->isChecked() ? 1 : 0) + (checkBox_ch2active->isChecked() ? 1 : 0) + (checkBox_ch3active->isChecked() ? 1 : 0) + (checkBox_ch4active->isChecked() ? 1 : 0);
    switch( actives ) {
    case 4 : if( spinBox_samplerate->value() > 100 ) {
                 spinBox_samplerate->setValue(100);
                 return true;
             }
             break;
    case 3 : if( spinBox_samplerate->value() > 125 ) {
                 spinBox_samplerate->setValue(125);
                 return true;
             }
             break;
    case 2 : if( spinBox_samplerate->value() > 200 ) {
                 spinBox_samplerate->setValue(200);
                 return true;
             }
             break;
    }
    return false;
}

void dockWidget_scope::submitSampleRate(int value)
{
    comboBox_samplerate->setCurrentIndex(comboBox_samplerate->findText(QString::number(value)));
    if( !checkSampleRate() )
        emit sampleRateChanged(value*1000);
}

void dockWidget_scope::updateColor(meta::colorindex ci, QColor color)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText,color);
    switch( ci ) {
    case meta::channel1 : checkBox_ch1active->setPalette(pal);
                          break;
    case meta::channel2 : checkBox_ch2active->setPalette(pal);
                          break;
    case meta::channel3 : checkBox_ch3active->setPalette(pal);
                          break;
    case meta::channel4 : checkBox_ch4active->setPalette(pal);
                          break;
    default             : break;
    }
}

// ########## DOCKWIDGET TRIGGER #########

dockWidget_trigger::dockWidget_trigger(QWidget *parent, Qt::WindowFlags flags) : QDockWidget(parent,flags), p_enabled_channels(0)
{
    setWindowTitle(tr("Trigger"));
    setObjectName("dockWidget_trigger");

    QWidget *widget = new QWidget(this);

    comboBox_edge = new QComboBox(widget);
    comboBox_edge->addItems(QStringList() << tr("Deaktiviert")
                                          << tr("Flanke egal")
                                          << tr("steigende Flanke")
                                          << tr("fallende Flanke"));

    comboBox_channel = new QComboBox(widget);
    comboBox_channel->setEnabled(false);

    spinBox_triggervoltage = new QDoubleSpinBox(widget);
    spinBox_triggervoltage->setRange(-40,40);
    spinBox_triggervoltage->setSuffix("V");
    spinBox_triggervoltage->setEnabled(false);

    QLabel* label_triggervoltage = new QLabel(tr("Triggerspannung:"),widget);

    QGridLayout *layout = new QGridLayout(widget);
    layout->addWidget(comboBox_edge,0,0);
    layout->addWidget(comboBox_channel,0,1);
    layout->addWidget(label_triggervoltage,1,0);
    layout->addWidget(spinBox_triggervoltage,1,1);

    setWidget(widget);

    qRegisterMetaType<meta::triggermode>("meta::triggermode");

    connect(comboBox_channel,SIGNAL(currentIndexChanged(int)),SLOT(comboBox_channel_changed(int)));
    connect(comboBox_edge,SIGNAL(currentIndexChanged(int)),SLOT(updateTriggerSpinBox(int)));
    connect(spinBox_triggervoltage,SIGNAL(valueChanged(double)),SLOT(submitTriggerMode(double)));
}

void dockWidget_trigger::restoreSettings()
{
    QSettings settings;
    comboBox_channel->setCurrentIndex(settings.value("trigger/channel",0).toInt());
    comboBox_edge->setCurrentIndex(settings.value("trigger/mode",0).toInt());
    spinBox_triggervoltage->setValue(settings.value("trigger/voltage",0.0).toDouble());
}

void dockWidget_trigger::saveSettings()
{
    QSettings settings;
    settings.setValue("trigger/channel",comboBox_channel->currentIndex());
    settings.setValue("trigger/mode",comboBox_edge->currentIndex());
    settings.setValue("trigger/voltage",spinBox_triggervoltage->value());
}

unsigned char dockWidget_trigger::getChannel() const
{
    unsigned char index = comboBox_channel->currentIndex()+1; //number of bits we need to find
    for(unsigned char bit = 0; bit < 4; bit++) {
        if( (1 << bit) & p_enabled_channels ) //bit number "bit" is active, increase index count
            index--;
        if( index == 0)
            return bit;
    }
    return 0; //should not happen
}

void dockWidget_trigger::comboBox_channel_changed(int index)
{
    Q_UNUSED(index);
    submitTriggerMode(spinBox_triggervoltage->value());
}

void dockWidget_trigger::updateTriggerSpinBox(int index)
{
    spinBox_triggervoltage->setEnabled(index != 0);
    comboBox_channel->setEnabled(index != 0);
    submitTriggerMode(spinBox_triggervoltage->value());
}

void dockWidget_trigger::submitTriggerMode(double value)
{
    emit triggerModeChanged((meta::triggermode)comboBox_edge->currentIndex(),value,getChannel());
}

//these slots are some dirty hacks to ensure content correctness in comboBox_channel
void dockWidget_trigger::channel1changed(bool activated)
{
    if( (p_enabled_channels & 1) == activated ) //channel already active/inactive
        return;

    if( activated ) {
        comboBox_channel->insertItem(0,tr("Kanal 1"));
        p_enabled_channels |= 1;
    } else {
        comboBox_channel->removeItem(0);
        p_enabled_channels &= ~1;
    }
}

void dockWidget_trigger::channel2changed(bool activated)
{
    if( (p_enabled_channels >> 1 & 1) == activated ) //channel already active/inactive
        return;

    const int index = p_enabled_channels & 1 ? 1 : 0;
    if( activated ) {
        comboBox_channel->insertItem(index,tr("Kanal 2"));
        p_enabled_channels |= 2;
    } else {
        comboBox_channel->removeItem(index);
        p_enabled_channels &= ~2;
    }
}

void dockWidget_trigger::channel3changed(bool activated)
{
    if( (p_enabled_channels >> 2 & 1) == activated ) //channel already active/inactive
        return;

    const int index = (p_enabled_channels & 1 ? 1 : 0) + (p_enabled_channels & 2 ? 1 : 0);
    if( activated ) {
        comboBox_channel->insertItem(index,tr("Kanal 3"));
        p_enabled_channels |= 4;
    } else {
        comboBox_channel->removeItem(index);
        p_enabled_channels &= ~4;
    }
}

void dockWidget_trigger::channel4changed(bool activated)
{
    if( (p_enabled_channels >> 3 & 1) == activated ) //channel already active/inactive
        return;

    const int index = (p_enabled_channels & 1 ? 1 : 0) + (p_enabled_channels & 2 ? 1 : 0) + (p_enabled_channels & 4 ? 1 : 0);
    if( activated ) {
        comboBox_channel->insertItem(index,tr("Kanal 4"));
        p_enabled_channels |= 8;
    } else {
        comboBox_channel->removeItem(index);
        p_enabled_channels &= ~8;
    }
}

// ########## DOCKWIDGET DATAVIEW #########

dockWidget_dataview::dockWidget_dataview(storage *datastorage, QWidget *parent, Qt::WindowFlags flags) : QDockWidget(parent,flags), p_storage(datastorage)
{
    setWindowTitle(tr("Datenspeicher"));
    setObjectName("dockWidget_dataview");

    QWidget *widget = new QWidget(this);
    spinBox_maxDatasets = new QSpinBox(widget);
    spinBox_maxDatasets->setRange(1,1000);
    QLabel *label_maxDatasets = new QLabel(tr("Höchstzahl Messbilder"),widget);
    listView_datasets = new QListView(widget);
    listView_datasets->setModel(p_storage);

    QHBoxLayout *layout_maxDatasets = new QHBoxLayout;
    layout_maxDatasets->addWidget(label_maxDatasets);
    layout_maxDatasets->addWidget(spinBox_maxDatasets);

    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(listView_datasets);
    layout->addLayout(layout_maxDatasets);

    connect(listView_datasets,SIGNAL(clicked(QModelIndex)),SLOT(submitShowDataset(QModelIndex)));
    connect(spinBox_maxDatasets,SIGNAL(valueChanged(int)),SIGNAL(maximumDatasetsChanged(int)));

    setWidget(widget);
}

void dockWidget_dataview::restoreSettings()
{
    QSettings settings;
    spinBox_maxDatasets->setValue(settings.value("dataview/maxdatasets",1).toInt());
}

void dockWidget_dataview::saveSettings()
{
    QSettings settings;
    settings.setValue("dataview/maxdatasets",spinBox_maxDatasets->value());
}

void dockWidget_dataview::submitShowDataset(QModelIndex index)
{
    emit showDataset(index.row());
}
