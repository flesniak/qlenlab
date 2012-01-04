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
#include <QMessageBox>

#include "qlenlab.h"
#include "ui_qlenlab.h"
#include "signaldata.h"
#include "settingsdialog.h"
#include "plot.h"
#include "communicator.h"
#include "dockwidgets.h"

QLenLab::QLenLab(QWidget *parent) : QMainWindow(parent), ui(new Ui::QLenLab)
{
    ui->setupUi(this);
    setWindowTitle(tr("QLenLab %1").arg(version));

    settingsdlg = NULL;

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    dockWidget_generator *dw_generator = new dockWidget_generator(this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_generator);

    dockWidget_viewport *dw_viewport = new dockWidget_viewport(this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_viewport);

    dockWidget_scope *dw_scope = new dockWidget_scope(this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_scope);

    com = new communicator(this);
    plotter = new plot(com,this);
    settingsdlg = new settingsdialog(com,this);

    label_connectionstatus = new QLabel(this);
    tabWidget->addTab(plotter,tr("Plot"));
    ui->statusBar->addPermanentWidget(label_connectionstatus);

    ui->action_settings->setIcon(QIcon::fromTheme("configure"));
    ui->action_quit->setIcon(QIcon::fromTheme("application-exit"));

    //various connects
    connect(com,SIGNAL(connectionStateChanged(bool)),SLOT(setConnectionStatus(bool)));

    //connects for dockWidget_scope
    connect(ui->comboBox_samplerate,SIGNAL(currentIndexChanged(int)),SLOT(submitSampleRate(int)));
    connect(ui->checkBox_ch1,SIGNAL(toggled(bool)),com,SLOT(setchannel1active(bool)));
    connect(ui->checkBox_ch2,SIGNAL(toggled(bool)),com,SLOT(setchannel2active(bool)));
    connect(ui->checkBox_ch3,SIGNAL(toggled(bool)),com,SLOT(setchannel3active(bool)));
    connect(ui->checkBox_ch4,SIGNAL(toggled(bool)),com,SLOT(setchannel4active(bool)));
    connect(ui->checkBox_ch1_alt,SIGNAL(toggled(bool)),com,SLOT(setchannel1offset(bool)));
    connect(ui->checkBox_ch2_alt,SIGNAL(toggled(bool)),com,SLOT(setchannel2offset(bool)));
    connect(ui->checkBox_ch3_alt,SIGNAL(toggled(bool)),com,SLOT(setchannel3offset(bool)));
    connect(ui->checkBox_ch4_alt,SIGNAL(toggled(bool)),com,SLOT(setchannel4offset(bool)));
    connect(ui->comboBox_range1,SIGNAL(currentIndexChanged(int)),com,SLOT(setrange1(int)));
    connect(ui->comboBox_range2,SIGNAL(currentIndexChanged(int)),com,SLOT(setrange2(int)));

    //connects for dockWidget_generator
    connect(ui->slider_sinus,SIGNAL(valueChanged(int)),SLOT(freqSliderSinChanged(int)));
    connect(ui->slider_sinus,SIGNAL(sliderReleased()),SLOT(submitSinusFreq()));
    connect(ui->slider_square,SIGNAL(valueChanged(int)),SLOT(freqSliderSqrChanged(int)));
    connect(ui->slider_square,SIGNAL(sliderReleased()),SLOT(submitSquareFreq()));
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
    connect(ui->action_start,SIGNAL(triggered()),SLOT(start()));
    connect(ui->action_stop,SIGNAL(triggered()),SLOT(stop()));

    setConnectionStatus(false);
    statusBar()->clearMessage();
    restoreSettings();

    ui->action_viewport->setChecked(ui->dockWidget_viewport->isVisible());
    ui->action_scope->setChecked(ui->dockWidget_scope->isVisible());
    plotter->updateViewportX(ui->comboBox_xaxis->currentText().toInt());
    plotter->updateViewportY(ui->doubleSpinBox_yaxis_lower->value(),ui->doubleSpinBox_yaxis_upper->value());

    //initCom();
}

QLenLab::~QLenLab()
{
    delete ui;
}

void QLenLab::initCom()
{
    qDebug() << "[qlenlab] init communicator";
    com->setactivechannels(ui->checkBox_ch1->isChecked() ? ch1a : 0 + ui->checkBox_ch2->isChecked() ? ch1b : 0 + ui->checkBox_ch3->isChecked() ? ch2a : 0 + ui->checkBox_ch4->isChecked() ? ch2b : 0);
    com->setoffset(ui->checkBox_ch1_alt->isChecked() ? ch1a : 0 + ui->checkBox_ch2_alt->isChecked() ? ch1b : 0 + ui->checkBox_ch3_alt->isChecked() ? ch2a : 0 + ui->checkBox_ch4_alt->isChecked() ? ch2b : 0);
    submitSinusFreq();
    submitSquareFreq();
    submitSquareRatio();
    submitSampleRate(ui->comboBox_samplerate->currentIndex());
    com->setrange1(ui->comboBox_range1->currentIndex());
    com->setrange2(ui->comboBox_range2->currentIndex());
}

void QLenLab::restoreSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray(),1);
    int xaxis = settings.value("viewport/xaxis",50).toInt();
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
    ui->checkBox_ch1->setChecked(settings.value("scope/ch1active",false).toBool());
    ui->checkBox_ch2->setChecked(settings.value("scope/ch2active",false).toBool());
    ui->checkBox_ch3->setChecked(settings.value("scope/ch3active",false).toBool());
    ui->checkBox_ch4->setChecked(settings.value("scope/ch4active",false).toBool());
    ui->checkBox_ch1_alt->setChecked(settings.value("scope/ch1alternate",false).toBool());
    ui->checkBox_ch2_alt->setChecked(settings.value("scope/ch2alternate",false).toBool());
    ui->checkBox_ch3_alt->setChecked(settings.value("scope/ch3alternate",false).toBool());
    ui->checkBox_ch4_alt->setChecked(settings.value("scope/ch4alternate",false).toBool());
    ui->checkBox_ch1_inv->setChecked(settings.value("scope/ch1invert",false).toBool());
    ui->checkBox_ch2_inv->setChecked(settings.value("scope/ch2invert",false).toBool());
    ui->checkBox_ch3_inv->setChecked(settings.value("scope/ch3invert",false).toBool());
    ui->checkBox_ch4_inv->setChecked(settings.value("scope/ch4invert",false).toBool());
    ui->doubleSpinBox_ch1_offset->setValue(settings.value("scope/ch1offset",0.0).toDouble());
    ui->doubleSpinBox_ch2_offset->setValue(settings.value("scope/ch2offset",0.0).toDouble());
    ui->doubleSpinBox_ch3_offset->setValue(settings.value("scope/ch3offset",0.0).toDouble());
    ui->doubleSpinBox_ch4_offset->setValue(settings.value("scope/ch4offset",0.0).toDouble());
    ui->comboBox_samplerate->setCurrentIndex(settings.value("scope/samplerate_index",2).toInt());
    ui->comboBox_range1->setCurrentIndex(settings.value("scope/range1_index",3).toInt());
    ui->comboBox_range2->setCurrentIndex(settings.value("scope/range2_index",3).toInt());
    ui->spinBox_sinus->setValue(settings.value("generator/sinus",1).toInt());
    ui->spinBox_square->setValue(settings.value("generator/square",1).toInt());
    ui->slider_square_ratio->setValue(settings.value("generator/squareratio",50).toInt());
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
    settings.setValue("scope/ch1active",ui->checkBox_ch1->isChecked());
    settings.setValue("scope/ch1alternate",ui->checkBox_ch1_alt->isChecked());
    settings.setValue("scope/ch1invert",ui->checkBox_ch1_inv->isChecked());
    settings.setValue("scope/ch1offset",ui->doubleSpinBox_ch1_offset->value());
    settings.setValue("scope/ch2active",ui->checkBox_ch2->isChecked());
    settings.setValue("scope/ch2alternate",ui->checkBox_ch2_alt->isChecked());
    settings.setValue("scope/ch2invert",ui->checkBox_ch2_inv->isChecked());
    settings.setValue("scope/ch2offset",ui->doubleSpinBox_ch2_offset->value());
    settings.setValue("scope/ch3active",ui->checkBox_ch3->isChecked());
    settings.setValue("scope/ch3alternate",ui->checkBox_ch3_alt->isChecked());
    settings.setValue("scope/ch3invert",ui->checkBox_ch3_inv->isChecked());
    settings.setValue("scope/ch3offset",ui->doubleSpinBox_ch3_offset->value());
    settings.setValue("scope/ch4active",ui->checkBox_ch4->isChecked());
    settings.setValue("scope/ch4alternate",ui->checkBox_ch4_alt->isChecked());
    settings.setValue("scope/ch4invert",ui->checkBox_ch4_inv->isChecked());
    settings.setValue("scope/ch4offset",ui->doubleSpinBox_ch4_offset->value());
    settings.setValue("scope/samplerate_index",ui->comboBox_samplerate->currentIndex());
    settings.setValue("generator/sinus",ui->spinBox_sinus->value());
    settings.setValue("generator/square",ui->spinBox_square->value());
    settings.setValue("generator/squareratio",ui->slider_square_ratio->value());
}

void QLenLab::quit()
{
    delete com;
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
        plotter->updateViewportX(value);
}

void QLenLab::viewportYChanged()
{
    double lower, upper;
    lower = ui->doubleSpinBox_yaxis_lower->value();
    upper = ui->doubleSpinBox_yaxis_upper->value();
    if( lower < upper )
        plotter->updateViewportY(lower, upper);
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
        submitSinusFreq();
        freqChanging.unlock();
    }
}

void QLenLab::freqBoxSqrChanged(int value) {
    if( freqChanging.tryLock() ) {
        if( ui->comboBox_range_square->currentIndex() == 0 )
            ui->slider_square->setValue(round(10.0*sqrt(value-(99.0/100))));
        else
            ui->slider_square->setValue(value);
        submitSquareFreq();
        freqChanging.unlock();
    }
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

void QLenLab::submitSampleRate(int index)
{
    switch(index) {
    case 0 : if( com->activechannelcount() <= 1 )
                 com->setsamplerate(400000);
             else
                 ui->comboBox_samplerate->setCurrentIndex(1);
             break;
    case 1 : if( com->activechannelcount() <= 2 )
                 com->setsamplerate(200000);
             else
                 ui->comboBox_samplerate->setCurrentIndex(2);
             break;
    case 2 : if( com->activechannelcount() <= 3 )
                 com->setsamplerate(125000);
             else
                 ui->comboBox_samplerate->setCurrentIndex(3);
             break;
    case 3 : com->setsamplerate(100000);
             break;
    case 4 : com->setsamplerate(50000);
             break;
    case 5 : com->setsamplerate(10000);
             break;
    case 6 : com->setsamplerate(1000);
             break;
    }
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
