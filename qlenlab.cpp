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

#include <QSettings>

#include "qlenlab.h"
#include "ui_qlenlab.h"
#include "signaldata.h"
#include "debugger.h"
#include "settingsdialog.h"
#include "plot.h"
#include "communicator.h"

QLenLab::QLenLab(QWidget *parent) : QMainWindow(parent), ui(new Ui::QLenLab)
{
    ui->setupUi(this);
    setWindowTitle(tr("QLenLab %1").arg(version));

    settingsdlg = NULL;

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    com = new communicator(this);
    plot = new Plot(com,this);
    settingsdlg = new settingsdialog(com,this);

    label_connectionstatus = new QLabel(this);
    tabWidget->addTab(plot,tr("Plot"));
    ui->statusBar->addPermanentWidget(label_connectionstatus);

    ui->action_settings->setIcon(QIcon::fromTheme("configure"));
    ui->action_quit->setIcon(QIcon::fromTheme("application-exit"));

    //various connects
    connect(com,SIGNAL(connectionStateChanged(bool)),SLOT(setConnectionStatus(bool)));
    connect(ui->action_start,SIGNAL(triggered()),SLOT(start()));
    connect(ui->action_stop,SIGNAL(triggered()),SLOT(stop()));

    //connects for dockWidget_scope
    connect(ui->checkBox_ch1,SIGNAL(toggled(bool)),com,SLOT(setchannel1active(bool)));
    connect(ui->checkBox_ch2,SIGNAL(toggled(bool)),com,SLOT(setchannel2active(bool)));
    connect(ui->checkBox_ch3,SIGNAL(toggled(bool)),com,SLOT(setchannel3active(bool)));
    connect(ui->checkBox_ch4,SIGNAL(toggled(bool)),com,SLOT(setchannel4active(bool)));

    //connects for dockWidget_generator
    connect(ui->slider_sinus,SIGNAL(valueChanged(int)),SLOT(freqSliderSinChanged(int)));
    connect(ui->slider_sinus,SIGNAL(sliderReleased()),SLOT(submitSinusFreq()));
    connect(ui->slider_square,SIGNAL(valueChanged(int)),SLOT(freqSliderSqrChanged(int)));
    connect(ui->slider_sinus,SIGNAL(sliderReleased()),SLOT(submitSquareFreq()));
    connect(ui->spinBox_sinus,SIGNAL(valueChanged(int)),SLOT(freqBoxSinChanged(int)));
    connect(ui->spinBox_square,SIGNAL(valueChanged(int)),SLOT(freqBoxSqrChanged(int)));
    connect(ui->comboBox_range_sinus,SIGNAL(currentIndexChanged(int)),SLOT(freqRangeSinChanged(int)));
    connect(ui->comboBox_range_square,SIGNAL(currentIndexChanged(int)),SLOT(freqRangeSqrChanged(int)));
    connect(ui->slider_square_ratio,SIGNAL(valueChanged(int)),SLOT(freqSliderSqrRatioChanged(int)));
    connect(ui->slider_square_ratio,SIGNAL(sliderReleased()),SLOT(submitSquareRatio()));

    //connects for dockWidget_viewport
    connect(ui->comboBox_xaxis,SIGNAL(currentIndexChanged(QString)),SLOT(viewportXChanged(QString)));
    connect(ui->doubleSpinBox_yaxis_lower,SIGNAL(valueChanged(double)),SLOT(viewportYChanged()));
    connect(ui->doubleSpinBox_yaxis_upper,SIGNAL(valueChanged(double)),SLOT(viewportYChanged()));

    //connects for dockWidget_trigger
    ui->action_trigger->setVisible(false); //not implemented yet

    //connect DockWidgets and corresponding menu actions
    connect(ui->action_viewport,SIGNAL(triggered(bool)),ui->dockWidget_viewport,SLOT(setShown(bool)));
    connect(ui->action_scope,SIGNAL(triggered(bool)),ui->dockWidget_scope,SLOT(setShown(bool)));
    connect(ui->action_generator,SIGNAL(triggered(bool)),ui->dockWidget_generator,SLOT(setShown(bool)));
    connect(ui->dockWidget_viewport,SIGNAL(visibilityChanged(bool)),ui->action_viewport,SLOT(setChecked(bool)));
    connect(ui->dockWidget_scope,SIGNAL(visibilityChanged(bool)),ui->action_scope,SLOT(setChecked(bool)));
    connect(ui->dockWidget_generator,SIGNAL(visibilityChanged(bool)),ui->action_generator,SLOT(setChecked(bool)));

    //connect other menu actions
    connect(ui->action_settings,SIGNAL(triggered()),SLOT(showSettings()));
    connect(ui->action_about,SIGNAL(triggered()),SLOT(about()));
    connect(ui->action_quit,SIGNAL(triggered()),SLOT(quit()));

    #ifdef USE_DEBUGGING_WINDOW
    debug = NULL;
    connect(ui->action_debug,SIGNAL(triggered()),SLOT(showDebug()));
    #else
    ui->action_debug->setVisible(false);
    #endif

    setConnectionStatus(false);
    statusBar()->clearMessage();
    restoreSettings();

    ui->action_viewport->setChecked(ui->dockWidget_viewport->isVisible());
    ui->action_scope->setChecked(ui->dockWidget_scope->isVisible());
    plot->updateViewportX(ui->comboBox_xaxis->currentText().toInt());
    plot->updateViewportY(ui->doubleSpinBox_yaxis_lower->value(),ui->doubleSpinBox_yaxis_upper->value());
}

QLenLab::~QLenLab()
{
    delete ui;
}

void QLenLab::restoreSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray(),1);
    int xaxis = settings.value("viewport/xaxis").toInt();
    if( xaxis != 0 ) {
        int index = ui->comboBox_xaxis->findText(QString::number(xaxis));
        if( index == -1 ) {
            ui->comboBox_xaxis->addItem(QString::number(index));
            ui->comboBox_xaxis->setCurrentIndex(ui->comboBox_xaxis->count()-1);
        }
        else
            ui->comboBox_xaxis->setCurrentIndex(index);
    }
    double yaxis_lower = settings.value("viewport/yaxis_lower").toDouble();
    double yaxis_upper = settings.value("viewport/yaxis_upper").toDouble();
    if( yaxis_lower < yaxis_upper) {
        ui->doubleSpinBox_yaxis_lower->setValue(yaxis_lower);
        ui->doubleSpinBox_yaxis_upper->setValue(yaxis_upper);
    }
    settingsdlg->restoreSettings();
}

void QLenLab::closeEvent(QCloseEvent *)
{
    QSettings settings;
    settings.setValue("mainwindow/geometry",saveGeometry());
    settings.setValue("mainwindow/state",saveState(1));
    settings.setValue("viewport/xaxis",ui->comboBox_xaxis->currentText().toInt());
    settings.setValue("viewport/yaxis_lower",ui->doubleSpinBox_yaxis_lower->value());
    settings.setValue("viewport/yaxis_upper",ui->doubleSpinBox_yaxis_upper->value());
}

void QLenLab::quit()
{
    com->closeport();
    close();
}

void QLenLab::start()
{
    com->start();
    ui->action_start->setEnabled(false);
    ui->action_stop->setEnabled(true);
}

void QLenLab::stop()
{
    com->stop();
    ui->action_start->setEnabled(true);
    ui->action_stop->setEnabled(false);
}

void QLenLab::viewportXChanged(QString str)
{
    bool ok;
    int value = str.toInt(&ok);
    if( ok )
        plot->updateViewportX(value);
}

void QLenLab::viewportYChanged()
{
    double lower, upper;
    lower = ui->doubleSpinBox_yaxis_lower->value();
    upper = ui->doubleSpinBox_yaxis_upper->value();
    if( lower < upper )
        plot->updateViewportY(lower, upper);
}

void QLenLab::showSettings()
{
    settingsdlg->exec();
}

void QLenLab::showDebug()
{
    #ifdef USE_DEBUGGING_WINDOW
    if( debug == NULL )
        debug = new debugger(this);
    debug->exec();
    #endif
}

void QLenLab::setConnectionStatus(bool connected)
{
    if( connected ) {
        label_connectionstatus->setText("<font color='green'><b>"+tr("Verbunden")+"</b></font>");
        statusBar()->showMessage(tr("Verbunden mit: ")+com->getid(),5000);
        ui->action_start->setEnabled(true);
        ui->action_stop->setEnabled(false);
    }
    else {
        label_connectionstatus->setText("<font color='red'><b>"+tr("Nicht verbunden")+"</b></font>");
        statusBar()->showMessage(tr("Keine Verbindung zur Karte über Schnittstelle \"")+com->lastTriedPort()+"\"",5000);
        ui->action_start->setEnabled(false);
        ui->action_stop->setEnabled(false);
    }
}

void QLenLab::freqSliderSinChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( ui->comboBox_range_sinus->currentIndex() == 0 )
            ui->spinBox_sinus->setValue(round(1.0*value*value/100+1));
        else
            ui->spinBox_sinus->setValue(value);
        freqChanging.unlock();
    }
}

void QLenLab::freqSliderSqrChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( ui->comboBox_range_square->currentIndex() == 0 )
            ui->spinBox_square->setValue(round(1.0*value*value/100+1));
        else
            ui->spinBox_square->setValue(value);
        freqChanging.unlock();
    }
}

void QLenLab::freqBoxSinChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( ui->comboBox_range_sinus->currentIndex() == 0 )
            ui->slider_sinus->setValue(round(10.0*sqrt(value-(99.0/100))));
        else
            ui->slider_sinus->setValue(value);
        freqChanging.unlock();
    }
    submitSinusFreq();
}

void QLenLab::freqBoxSqrChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( ui->comboBox_range_square->currentIndex() == 0 )
            ui->slider_square->setValue(round(10.0*sqrt(value-(99.0/100))));
        else
            ui->slider_square->setValue(value);
        freqChanging.unlock();
    }
    submitSquareFreq();
}

void QLenLab::freqRangeSinChanged(int index)
{
    freqChanging.lock();
    switch( index ) {
    case 0 : ui->spinBox_sinus->setRange(1,10000);
             ui->slider_sinus->setRange(1,1000);
             break;
    case 1 : ui->spinBox_sinus->setRange(1,100);
             ui->slider_sinus->setRange(1,100);
             break;
    case 2 : ui->spinBox_sinus->setRange(100,1000);
             ui->slider_sinus->setRange(100,1000);
             break;
    case 3 : ui->spinBox_sinus->setRange(1000,10000);
             ui->slider_sinus->setRange(1000,10000);
             break;
    default: break;
    }
    freqChanging.unlock();
    freqBoxSinChanged(ui->spinBox_sinus->value());
}

void QLenLab::freqRangeSqrChanged(int index)
{
    freqChanging.lock();
    switch( index ) {
    case 0 : ui->spinBox_square->setRange(1,10000);
             ui->slider_square->setRange(1,1000);
             break;
    case 1 : ui->spinBox_square->setRange(1,100);
             ui->slider_square->setRange(1,100);
             break;
    case 2 : ui->spinBox_square->setRange(100,1000);
             ui->slider_square->setRange(100,1000);
             break;
    case 3 : ui->spinBox_square->setRange(1000,10000);
             ui->slider_square->setRange(1000,10000);
             break;
    default: break;
    }
    freqChanging.unlock();
    freqBoxSqrChanged(ui->spinBox_square->value());
}

void QLenLab::freqSliderSqrRatioChanged(int value)
{
    //ui->label_square_ratio->setText(QString::number(value)+":"+QString::number(ui->slider_square_ratio->maximum()-value+1));
    ui->label_square_ratio->setText(QString::number(value)+"%");
}

void QLenLab::submitSinusFreq()
{
    com->setsinusfrequency(ui->spinBox_sinus->value());
}

void QLenLab::submitSquareFreq()
{
    com->setsquarefrequency(ui->spinBox_square->value());
}

void QLenLab::submitSquareRatio()
{
    com->setsquareratio(ui->slider_square_ratio->value());
}

void QLenLab::about()
{
    QMessageBox::information(this,tr("Über QLenLab"),
                             tr("QLenLab ist ein Softwareprojekt, das die Ansteuerung der Hardware-Platine LENlab des Instituts für Biomedizinische Technik (IBT, http://ibt.kit.edu) "
                                "am Karlsruhe Institute of Technology (KIT) "
                                "ermöglichen soll.\n\nEs steht in keinerlei Zusammenhang mit den Entwicklern dieser Platform. Die Nutzung von QLenLab erfolgt auf eigene Gefahr, "
                                "allgemein gilt:\n\nQLenLab is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by "
                                "the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n"
                                "QLenLab is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
                                "See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with QLenLab. "
                                "If not, see <http://www.gnu.org/licenses/>.\n\nQLenLab is based in part on the work of the Qwt project (http://qwt.sf.net)."));
}
