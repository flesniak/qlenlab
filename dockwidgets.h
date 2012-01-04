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
    void viewportXChanged(int);
    void viewportYChanged(double,double);
};

class dockWidget_scope : public QDockWidget
{
    Q_OBJECT
public:
    dockWidget_scope(QWidget *parent = 0, Qt::WindowFlags flags = 0);
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
    void sampleRateChanged(int);
};

class dockWidget_trigger : public QDockWidget
{

};

class Ui_QLenLab_meta
{
public:
    QAction *action_quit;
    QAction *action_start;
    QAction *action_stop;
    QAction *action_about;
    QAction *action_scope;
    QAction *action_generator;
    QAction *action_trigger;
    QAction *action_viewport;
    QAction *action_settings;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_lenlab;
    QMenu *menu_measurement;
    QMenu *menu_interface;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_scope;
    QWidget *dockWidgetContents_scope;
    QGridLayout *gridLayout_2;
    QLabel *label_samplerate;
    QComboBox *comboBox_samplerate;
    QLabel *label_offset;
    QLabel *label_alt;
    QLabel *label_inv;
    QCheckBox *checkBox_ch1;
    QDoubleSpinBox *doubleSpinBox_ch1_offset;
    QCheckBox *checkBox_ch1_alt;
    QCheckBox *checkBox_ch1_inv;
    QCheckBox *checkBox_ch2;
    QDoubleSpinBox *doubleSpinBox_ch2_offset;
    QCheckBox *checkBox_ch2_alt;
    QCheckBox *checkBox_ch2_inv;
    QCheckBox *checkBox_ch3;
    QDoubleSpinBox *doubleSpinBox_ch3_offset;
    QCheckBox *checkBox_ch3_alt;
    QCheckBox *checkBox_ch3_inv;
    QCheckBox *checkBox_ch4;
    QDoubleSpinBox *doubleSpinBox_ch4_offset;
    QCheckBox *checkBox_ch4_alt;
    QGridLayout *gridLayout;
    QLabel *label_range1;
    QLabel *label_range2;
    QComboBox *comboBox_range1;
    QComboBox *comboBox_range2;
    QSpacerItem *verticalSpacer_2;
    QCheckBox *checkBox_ch4_inv;
    QDockWidget *dockWidget_viewport;
    QWidget *dockWidgetContents_viewport;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_viewport_xaxis;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBox_xaxis;
    QLabel *label_xaxi_unit;
    QGroupBox *groupBox_viewport_yaxis;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QDoubleSpinBox *doubleSpinBox_yaxis_lower;
    QLabel *label_xaxis_unit_lower;
    QLabel *label_yaxis_dash;
    QDoubleSpinBox *doubleSpinBox_yaxis_upper;
    QLabel *label_xaxis_unit_upper;
    QCheckBox *checkBox_yaxis_autoscale;
    QSpacerItem *verticalSpacer;
    QDockWidget *dockWidget_generator;
    QWidget *dockWidgetContents_generator;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget_generator;
    QWidget *tab_generator_sinus;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labe_sinus_freq;
    QSlider *slider_sinus;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBox_range_sinus;
    QSpinBox *spinBox_sinus;
    QSpacerItem *horizontalSpacer;
    QWidget *tab_generator_square;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labe_square_freq_2;
    QSlider *slider_square;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comboBox_range_square;
    QSpinBox *spinBox_square;
    QHBoxLayout *horizontalLayout_7;
    QLabel *labe_square_ratio_dummy;
    QSlider *slider_square_ratio;
    QLabel *label_square_ratio;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QMainWindow *QLenLab)
    {
        if (QLenLab->objectName().isEmpty())
            QLenLab->setObjectName(QString::fromUtf8("QLenLab"));
        QLenLab->resize(851, 624);
        QLenLab->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        action_quit = new QAction(QLenLab);
        action_quit->setObjectName(QString::fromUtf8("action_quit"));
        action_start = new QAction(QLenLab);
        action_start->setObjectName(QString::fromUtf8("action_start"));
        action_start->setEnabled(false);
        action_stop = new QAction(QLenLab);
        action_stop->setObjectName(QString::fromUtf8("action_stop"));
        action_stop->setEnabled(false);
        action_about = new QAction(QLenLab);
        action_about->setObjectName(QString::fromUtf8("action_about"));
        action_scope = new QAction(QLenLab);
        action_scope->setObjectName(QString::fromUtf8("action_scope"));
        action_scope->setCheckable(true);
        action_generator = new QAction(QLenLab);
        action_generator->setObjectName(QString::fromUtf8("action_generator"));
        action_generator->setCheckable(true);
        action_trigger = new QAction(QLenLab);
        action_trigger->setObjectName(QString::fromUtf8("action_trigger"));
        action_trigger->setCheckable(true);
        action_viewport = new QAction(QLenLab);
        action_viewport->setObjectName(QString::fromUtf8("action_viewport"));
        action_viewport->setCheckable(true);
        action_settings = new QAction(QLenLab);
        action_settings->setObjectName(QString::fromUtf8("action_settings"));
        centralWidget = new QWidget(QLenLab);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QLenLab->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QLenLab);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 851, 20));
        menu_lenlab = new QMenu(menuBar);
        menu_lenlab->setObjectName(QString::fromUtf8("menu_lenlab"));
        menu_measurement = new QMenu(menuBar);
        menu_measurement->setObjectName(QString::fromUtf8("menu_measurement"));
        menu_interface = new QMenu(menuBar);
        menu_interface->setObjectName(QString::fromUtf8("menu_interface"));
        QLenLab->setMenuBar(menuBar);
        statusBar = new QStatusBar(QLenLab);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QLenLab->setStatusBar(statusBar);
        dockWidget_scope = new QDockWidget(QLenLab);
        dockWidget_scope->setObjectName(QString::fromUtf8("dockWidget_scope"));
        dockWidgetContents_scope = new QWidget();
        dockWidgetContents_scope->setObjectName(QString::fromUtf8("dockWidgetContents_scope"));
        gridLayout_2 = new QGridLayout(dockWidgetContents_scope);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_samplerate = new QLabel(dockWidgetContents_scope);
        label_samplerate->setObjectName(QString::fromUtf8("label_samplerate"));

        gridLayout_2->addWidget(label_samplerate, 0, 0, 1, 1);

        comboBox_samplerate = new QComboBox(dockWidgetContents_scope);
        comboBox_samplerate->setObjectName(QString::fromUtf8("comboBox_samplerate"));

        gridLayout_2->addWidget(comboBox_samplerate, 0, 1, 1, 3);

        label_offset = new QLabel(dockWidgetContents_scope);
        label_offset->setObjectName(QString::fromUtf8("label_offset"));

        gridLayout_2->addWidget(label_offset, 1, 1, 1, 1);

        label_alt = new QLabel(dockWidgetContents_scope);
        label_alt->setObjectName(QString::fromUtf8("label_alt"));
        label_alt->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_alt, 1, 2, 1, 1);

        label_inv = new QLabel(dockWidgetContents_scope);
        label_inv->setObjectName(QString::fromUtf8("label_inv"));

        gridLayout_2->addWidget(label_inv, 1, 3, 1, 1);

        checkBox_ch1 = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch1->setObjectName(QString::fromUtf8("checkBox_ch1"));

        gridLayout_2->addWidget(checkBox_ch1, 2, 0, 1, 1);

        doubleSpinBox_ch1_offset = new QDoubleSpinBox(dockWidgetContents_scope);
        doubleSpinBox_ch1_offset->setObjectName(QString::fromUtf8("doubleSpinBox_ch1_offset"));
        doubleSpinBox_ch1_offset->setMinimum(-40);
        doubleSpinBox_ch1_offset->setMaximum(40);

        gridLayout_2->addWidget(doubleSpinBox_ch1_offset, 2, 1, 1, 1);

        checkBox_ch1_alt = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch1_alt->setObjectName(QString::fromUtf8("checkBox_ch1_alt"));

        gridLayout_2->addWidget(checkBox_ch1_alt, 2, 2, 1, 1);

        checkBox_ch1_inv = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch1_inv->setObjectName(QString::fromUtf8("checkBox_ch1_inv"));

        gridLayout_2->addWidget(checkBox_ch1_inv, 2, 3, 1, 1);

        checkBox_ch2 = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch2->setObjectName(QString::fromUtf8("checkBox_ch2"));

        gridLayout_2->addWidget(checkBox_ch2, 3, 0, 1, 1);

        doubleSpinBox_ch2_offset = new QDoubleSpinBox(dockWidgetContents_scope);
        doubleSpinBox_ch2_offset->setObjectName(QString::fromUtf8("doubleSpinBox_ch2_offset"));
        doubleSpinBox_ch2_offset->setMinimum(-40);
        doubleSpinBox_ch2_offset->setMaximum(40);

        gridLayout_2->addWidget(doubleSpinBox_ch2_offset, 3, 1, 1, 1);

        checkBox_ch2_alt = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch2_alt->setObjectName(QString::fromUtf8("checkBox_ch2_alt"));

        gridLayout_2->addWidget(checkBox_ch2_alt, 3, 2, 1, 1);

        checkBox_ch2_inv = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch2_inv->setObjectName(QString::fromUtf8("checkBox_ch2_inv"));

        gridLayout_2->addWidget(checkBox_ch2_inv, 3, 3, 1, 1);

        checkBox_ch3 = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch3->setObjectName(QString::fromUtf8("checkBox_ch3"));

        gridLayout_2->addWidget(checkBox_ch3, 4, 0, 1, 1);

        doubleSpinBox_ch3_offset = new QDoubleSpinBox(dockWidgetContents_scope);
        doubleSpinBox_ch3_offset->setObjectName(QString::fromUtf8("doubleSpinBox_ch3_offset"));
        doubleSpinBox_ch3_offset->setMinimum(-40);
        doubleSpinBox_ch3_offset->setMaximum(40);

        gridLayout_2->addWidget(doubleSpinBox_ch3_offset, 4, 1, 1, 1);

        checkBox_ch3_alt = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch3_alt->setObjectName(QString::fromUtf8("checkBox_ch3_alt"));

        gridLayout_2->addWidget(checkBox_ch3_alt, 4, 2, 1, 1);

        checkBox_ch3_inv = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch3_inv->setObjectName(QString::fromUtf8("checkBox_ch3_inv"));

        gridLayout_2->addWidget(checkBox_ch3_inv, 4, 3, 1, 1);

        checkBox_ch4 = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch4->setObjectName(QString::fromUtf8("checkBox_ch4"));

        gridLayout_2->addWidget(checkBox_ch4, 6, 0, 1, 1);

        doubleSpinBox_ch4_offset = new QDoubleSpinBox(dockWidgetContents_scope);
        doubleSpinBox_ch4_offset->setObjectName(QString::fromUtf8("doubleSpinBox_ch4_offset"));
        doubleSpinBox_ch4_offset->setMinimum(-40);
        doubleSpinBox_ch4_offset->setMaximum(40);

        gridLayout_2->addWidget(doubleSpinBox_ch4_offset, 6, 1, 1, 1);

        checkBox_ch4_alt = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch4_alt->setObjectName(QString::fromUtf8("checkBox_ch4_alt"));

        gridLayout_2->addWidget(checkBox_ch4_alt, 6, 2, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_range1 = new QLabel(dockWidgetContents_scope);
        label_range1->setObjectName(QString::fromUtf8("label_range1"));

        gridLayout->addWidget(label_range1, 0, 0, 1, 1);

        label_range2 = new QLabel(dockWidgetContents_scope);
        label_range2->setObjectName(QString::fromUtf8("label_range2"));

        gridLayout->addWidget(label_range2, 0, 1, 1, 1);

        comboBox_range1 = new QComboBox(dockWidgetContents_scope);
        comboBox_range1->setObjectName(QString::fromUtf8("comboBox_range1"));

        gridLayout->addWidget(comboBox_range1, 1, 0, 1, 1);

        comboBox_range2 = new QComboBox(dockWidgetContents_scope);
        comboBox_range2->setObjectName(QString::fromUtf8("comboBox_range2"));

        gridLayout->addWidget(comboBox_range2, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 7, 0, 1, 3);

        verticalSpacer_2 = new QSpacerItem(20, 26, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 8, 0, 1, 1);

        checkBox_ch4_inv = new QCheckBox(dockWidgetContents_scope);
        checkBox_ch4_inv->setObjectName(QString::fromUtf8("checkBox_ch4_inv"));

        gridLayout_2->addWidget(checkBox_ch4_inv, 6, 3, 1, 1);

        dockWidget_scope->setWidget(dockWidgetContents_scope);
        QLenLab->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_scope);
        dockWidget_viewport = new QDockWidget(QLenLab);
        dockWidget_viewport->setObjectName(QString::fromUtf8("dockWidget_viewport"));
        dockWidget_viewport->setFloating(false);
        dockWidgetContents_viewport = new QWidget();
        dockWidgetContents_viewport->setObjectName(QString::fromUtf8("dockWidgetContents_viewport"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_viewport);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_viewport_xaxis = new QGroupBox(dockWidgetContents_viewport);
        groupBox_viewport_xaxis->setObjectName(QString::fromUtf8("groupBox_viewport_xaxis"));
        horizontalLayout = new QHBoxLayout(groupBox_viewport_xaxis);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        comboBox_xaxis = new QComboBox(groupBox_viewport_xaxis);
        comboBox_xaxis->setObjectName(QString::fromUtf8("comboBox_xaxis"));

        horizontalLayout->addWidget(comboBox_xaxis);

        label_xaxi_unit = new QLabel(groupBox_viewport_xaxis);
        label_xaxi_unit->setObjectName(QString::fromUtf8("label_xaxi_unit"));

        horizontalLayout->addWidget(label_xaxi_unit);


        verticalLayout_2->addWidget(groupBox_viewport_xaxis);

        groupBox_viewport_yaxis = new QGroupBox(dockWidgetContents_viewport);
        groupBox_viewport_yaxis->setObjectName(QString::fromUtf8("groupBox_viewport_yaxis"));
        verticalLayout = new QVBoxLayout(groupBox_viewport_yaxis);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        doubleSpinBox_yaxis_lower = new QDoubleSpinBox(groupBox_viewport_yaxis);
        doubleSpinBox_yaxis_lower->setObjectName(QString::fromUtf8("doubleSpinBox_yaxis_lower"));
        doubleSpinBox_yaxis_lower->setMinimum(-35);
        doubleSpinBox_yaxis_lower->setMaximum(35);
        doubleSpinBox_yaxis_lower->setValue(-5);

        horizontalLayout_2->addWidget(doubleSpinBox_yaxis_lower);

        label_xaxis_unit_lower = new QLabel(groupBox_viewport_yaxis);
        label_xaxis_unit_lower->setObjectName(QString::fromUtf8("label_xaxis_unit_lower"));

        horizontalLayout_2->addWidget(label_xaxis_unit_lower);

        label_yaxis_dash = new QLabel(groupBox_viewport_yaxis);
        label_yaxis_dash->setObjectName(QString::fromUtf8("label_yaxis_dash"));
        label_yaxis_dash->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_yaxis_dash);

        doubleSpinBox_yaxis_upper = new QDoubleSpinBox(groupBox_viewport_yaxis);
        doubleSpinBox_yaxis_upper->setObjectName(QString::fromUtf8("doubleSpinBox_yaxis_upper"));
        doubleSpinBox_yaxis_upper->setMinimum(-35);
        doubleSpinBox_yaxis_upper->setMaximum(35);
        doubleSpinBox_yaxis_upper->setValue(5);

        horizontalLayout_2->addWidget(doubleSpinBox_yaxis_upper);

        label_xaxis_unit_upper = new QLabel(groupBox_viewport_yaxis);
        label_xaxis_unit_upper->setObjectName(QString::fromUtf8("label_xaxis_unit_upper"));

        horizontalLayout_2->addWidget(label_xaxis_unit_upper);


        verticalLayout->addLayout(horizontalLayout_2);

        checkBox_yaxis_autoscale = new QCheckBox(groupBox_viewport_yaxis);
        checkBox_yaxis_autoscale->setObjectName(QString::fromUtf8("checkBox_yaxis_autoscale"));
        checkBox_yaxis_autoscale->setEnabled(false);

        verticalLayout->addWidget(checkBox_yaxis_autoscale);


        verticalLayout_2->addWidget(groupBox_viewport_yaxis);

        verticalSpacer = new QSpacerItem(20, 61, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        dockWidget_viewport->setWidget(dockWidgetContents_viewport);
        QLenLab->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_viewport);
        dockWidget_generator = new QDockWidget(QLenLab);
        dockWidget_generator->setObjectName(QString::fromUtf8("dockWidget_generator"));
        dockWidgetContents_generator = new QWidget();
        dockWidgetContents_generator->setObjectName(QString::fromUtf8("dockWidgetContents_generator"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents_generator);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tabWidget_generator = new QTabWidget(dockWidgetContents_generator);
        tabWidget_generator->setObjectName(QString::fromUtf8("tabWidget_generator"));
        tab_generator_sinus = new QWidget();
        tab_generator_sinus->setObjectName(QString::fromUtf8("tab_generator_sinus"));
        verticalLayout_3 = new QVBoxLayout(tab_generator_sinus);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        labe_sinus_freq = new QLabel(tab_generator_sinus);
        labe_sinus_freq->setObjectName(QString::fromUtf8("labe_sinus_freq"));

        horizontalLayout_5->addWidget(labe_sinus_freq);

        slider_sinus = new QSlider(tab_generator_sinus);
        slider_sinus->setObjectName(QString::fromUtf8("slider_sinus"));
        slider_sinus->setMinimum(1);
        slider_sinus->setMaximum(1000);
        slider_sinus->setPageStep(100);
        slider_sinus->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(slider_sinus);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        comboBox_range_sinus = new QComboBox(tab_generator_sinus);
        comboBox_range_sinus->setObjectName(QString::fromUtf8("comboBox_range_sinus"));

        horizontalLayout_4->addWidget(comboBox_range_sinus);

        spinBox_sinus = new QSpinBox(tab_generator_sinus);
        spinBox_sinus->setObjectName(QString::fromUtf8("spinBox_sinus"));
        spinBox_sinus->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_sinus->setMinimum(1);
        spinBox_sinus->setMaximum(10000);

        horizontalLayout_4->addWidget(spinBox_sinus);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_4);

        tabWidget_generator->addTab(tab_generator_sinus, QString());
        tab_generator_square = new QWidget();
        tab_generator_square->setObjectName(QString::fromUtf8("tab_generator_square"));
        verticalLayout_4 = new QVBoxLayout(tab_generator_square);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        labe_square_freq_2 = new QLabel(tab_generator_square);
        labe_square_freq_2->setObjectName(QString::fromUtf8("labe_square_freq_2"));

        horizontalLayout_6->addWidget(labe_square_freq_2);

        slider_square = new QSlider(tab_generator_square);
        slider_square->setObjectName(QString::fromUtf8("slider_square"));
        slider_square->setMinimum(1);
        slider_square->setMaximum(1000);
        slider_square->setPageStep(100);
        slider_square->setOrientation(Qt::Horizontal);
        slider_square->setInvertedAppearance(false);
        slider_square->setInvertedControls(false);

        horizontalLayout_6->addWidget(slider_square);


        verticalLayout_4->addLayout(horizontalLayout_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        comboBox_range_square = new QComboBox(tab_generator_square);
        comboBox_range_square->setObjectName(QString::fromUtf8("comboBox_range_square"));

        horizontalLayout_3->addWidget(comboBox_range_square);

        spinBox_square = new QSpinBox(tab_generator_square);
        spinBox_square->setObjectName(QString::fromUtf8("spinBox_square"));
        spinBox_square->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_square->setMinimum(1);
        spinBox_square->setMaximum(10000);

        horizontalLayout_3->addWidget(spinBox_square);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        labe_square_ratio_dummy = new QLabel(tab_generator_square);
        labe_square_ratio_dummy->setObjectName(QString::fromUtf8("labe_square_ratio_dummy"));

        horizontalLayout_7->addWidget(labe_square_ratio_dummy);

        slider_square_ratio = new QSlider(tab_generator_square);
        slider_square_ratio->setObjectName(QString::fromUtf8("slider_square_ratio"));
        slider_square_ratio->setMinimum(1);
        slider_square_ratio->setValue(50);
        slider_square_ratio->setOrientation(Qt::Horizontal);
        slider_square_ratio->setTickPosition(QSlider::NoTicks);

        horizontalLayout_7->addWidget(slider_square_ratio);

        label_square_ratio = new QLabel(tab_generator_square);
        label_square_ratio->setObjectName(QString::fromUtf8("label_square_ratio"));

        horizontalLayout_7->addWidget(label_square_ratio);


        verticalLayout_4->addLayout(horizontalLayout_7);

        tabWidget_generator->addTab(tab_generator_square, QString());

        verticalLayout_5->addWidget(tabWidget_generator);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        dockWidget_generator->setWidget(dockWidgetContents_generator);
        QLenLab->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_generator);

        menuBar->addAction(menu_lenlab->menuAction());
        menuBar->addAction(menu_interface->menuAction());
        menuBar->addAction(menu_measurement->menuAction());
        menu_lenlab->addAction(action_settings);
        menu_lenlab->addSeparator();
        menu_lenlab->addAction(action_about);
        menu_lenlab->addAction(action_quit);
        menu_measurement->addAction(action_start);
        menu_measurement->addAction(action_stop);
        menu_interface->addAction(action_scope);
        menu_interface->addAction(action_generator);
        menu_interface->addAction(action_trigger);
        menu_interface->addAction(action_viewport);

        retranslateUi(QLenLab);

        comboBox_range1->setCurrentIndex(3);
        comboBox_range2->setCurrentIndex(3);
        tabWidget_generator->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QLenLab);
    } // setupUi

    void retranslateUi(QMainWindow *QLenLab)
    {
        QLenLab->setWindowTitle(QApplication::translate("QLenLab", "QLenLab", 0, QApplication::UnicodeUTF8));
        action_quit->setText(QApplication::translate("QLenLab", "Beenden", 0, QApplication::UnicodeUTF8));
        action_start->setText(QApplication::translate("QLenLab", "Starten", 0, QApplication::UnicodeUTF8));
        action_stop->setText(QApplication::translate("QLenLab", "Stoppen", 0, QApplication::UnicodeUTF8));
        action_about->setText(QApplication::translate("QLenLab", "\303\234ber QLenLab", 0, QApplication::UnicodeUTF8));
        action_scope->setText(QApplication::translate("QLenLab", "Oszilloskop", 0, QApplication::UnicodeUTF8));
        action_generator->setText(QApplication::translate("QLenLab", "Frequenzgenerator", 0, QApplication::UnicodeUTF8));
        action_trigger->setText(QApplication::translate("QLenLab", "Trigger", 0, QApplication::UnicodeUTF8));
        action_viewport->setText(QApplication::translate("QLenLab", "Anzeigeparameter", 0, QApplication::UnicodeUTF8));
        action_settings->setText(QApplication::translate("QLenLab", "Einstellungen", 0, QApplication::UnicodeUTF8));
        menu_lenlab->setTitle(QApplication::translate("QLenLab", "QLenlab", 0, QApplication::UnicodeUTF8));
        menu_measurement->setTitle(QApplication::translate("QLenLab", "Messung", 0, QApplication::UnicodeUTF8));
        menu_interface->setTitle(QApplication::translate("QLenLab", "Anzeige", 0, QApplication::UnicodeUTF8));
        dockWidget_scope->setWindowTitle(QApplication::translate("QLenLab", "Oszilloskop", 0, QApplication::UnicodeUTF8));
        label_samplerate->setText(QApplication::translate("QLenLab", "Samplerate", 0, QApplication::UnicodeUTF8));
        comboBox_samplerate->clear();
        comboBox_samplerate->insertItems(0, QStringList()
         << QApplication::translate("QLenLab", "400kHz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "200kHz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "125kHz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "100kHz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "50kHz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "10kHz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1kHz", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        label_offset->setToolTip(QApplication::translate("QLenLab", "Manuelles Offset. Addiert den eingestellten Wert in Volt auf die Spannung des Kanals.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_offset->setText(QApplication::translate("QLenLab", "Offset", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_alt->setToolTip(QApplication::translate("QLenLab", "Macht den Eingang symmetrisch, d.h. es wird m\303\266glich, negative Spannungen zu messen.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_alt->setText(QApplication::translate("QLenLab", "~", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_inv->setToolTip(QApplication::translate("QLenLab", "Invertiert die Spannung des jeweiligen Kanals", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_inv->setText(QApplication::translate("QLenLab", "Invert", 0, QApplication::UnicodeUTF8));
        checkBox_ch1->setText(QApplication::translate("QLenLab", "Kanal 1", 0, QApplication::UnicodeUTF8));
        checkBox_ch1_alt->setText(QString());
        checkBox_ch1_inv->setText(QString());
        checkBox_ch2->setText(QApplication::translate("QLenLab", "Kanal 2", 0, QApplication::UnicodeUTF8));
        checkBox_ch2_alt->setText(QString());
        checkBox_ch2_inv->setText(QString());
        checkBox_ch3->setText(QApplication::translate("QLenLab", "Kanal 3", 0, QApplication::UnicodeUTF8));
        checkBox_ch3_alt->setText(QString());
        checkBox_ch3_inv->setText(QString());
        checkBox_ch4->setText(QApplication::translate("QLenLab", "Kanal 4", 0, QApplication::UnicodeUTF8));
        checkBox_ch4_alt->setText(QString());
        label_range1->setText(QApplication::translate("QLenLab", "Range 1/2", 0, QApplication::UnicodeUTF8));
        label_range2->setText(QApplication::translate("QLenLab", "Range 3/4", 0, QApplication::UnicodeUTF8));
        comboBox_range1->clear();
        comboBox_range1->insertItems(0, QStringList()
         << QApplication::translate("QLenLab", "0.5x (1.6V)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1.0x (3.3V)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "2.0x (6.6V)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "10x (33V)", 0, QApplication::UnicodeUTF8)
        );
        comboBox_range2->clear();
        comboBox_range2->insertItems(0, QStringList()
         << QApplication::translate("QLenLab", "0.5x (1.6V)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1.0x (3.3V)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "2.0x (6.6V)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "10x (33V)", 0, QApplication::UnicodeUTF8)
        );
        checkBox_ch4_inv->setText(QString());
        dockWidget_viewport->setWindowTitle(QApplication::translate("QLenLab", "Anzeige", 0, QApplication::UnicodeUTF8));
        groupBox_viewport_xaxis->setTitle(QApplication::translate("QLenLab", "X-Achse", 0, QApplication::UnicodeUTF8));
        comboBox_xaxis->clear();
        comboBox_xaxis->insertItems(0, QStringList()
         << QApplication::translate("QLenLab", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "50", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "100", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "500", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "2000", 0, QApplication::UnicodeUTF8)
        );
        label_xaxi_unit->setText(QApplication::translate("QLenLab", "ms", 0, QApplication::UnicodeUTF8));
        groupBox_viewport_yaxis->setTitle(QApplication::translate("QLenLab", "Y-Achse", 0, QApplication::UnicodeUTF8));
        label_xaxis_unit_lower->setText(QApplication::translate("QLenLab", "V", 0, QApplication::UnicodeUTF8));
        label_yaxis_dash->setText(QApplication::translate("QLenLab", "-", 0, QApplication::UnicodeUTF8));
        label_xaxis_unit_upper->setText(QApplication::translate("QLenLab", "V", 0, QApplication::UnicodeUTF8));
        checkBox_yaxis_autoscale->setText(QApplication::translate("QLenLab", "Autoscale", 0, QApplication::UnicodeUTF8));
        dockWidget_generator->setWindowTitle(QApplication::translate("QLenLab", "Frequenzgenerator", 0, QApplication::UnicodeUTF8));
        labe_sinus_freq->setText(QApplication::translate("QLenLab", "Frequenz", 0, QApplication::UnicodeUTF8));
        comboBox_range_sinus->clear();
        comboBox_range_sinus->insertItems(0, QStringList()
         << QApplication::translate("QLenLab", "1-10000Hz exp", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1-100Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "100-1000Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1000-10000Hz", 0, QApplication::UnicodeUTF8)
        );
        spinBox_sinus->setSuffix(QApplication::translate("QLenLab", "Hz", 0, QApplication::UnicodeUTF8));
        tabWidget_generator->setTabText(tabWidget_generator->indexOf(tab_generator_sinus), QApplication::translate("QLenLab", "Sinus", 0, QApplication::UnicodeUTF8));
        labe_square_freq_2->setText(QApplication::translate("QLenLab", "Frequenz", 0, QApplication::UnicodeUTF8));
        comboBox_range_square->clear();
        comboBox_range_square->insertItems(0, QStringList()
         << QApplication::translate("QLenLab", "1-10000Hz exp", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1-100Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "100-1000Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QLenLab", "1000-10000Hz", 0, QApplication::UnicodeUTF8)
        );
        spinBox_square->setSuffix(QApplication::translate("QLenLab", "Hz", 0, QApplication::UnicodeUTF8));
        labe_square_ratio_dummy->setText(QApplication::translate("QLenLab", "High-State", 0, QApplication::UnicodeUTF8));
        label_square_ratio->setText(QApplication::translate("QLenLab", "50%", 0, QApplication::UnicodeUTF8));
        tabWidget_generator->setTabText(tabWidget_generator->indexOf(tab_generator_square), QApplication::translate("QLenLab", "Rechteck", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

#endif // DOCKWIDGETS_H
