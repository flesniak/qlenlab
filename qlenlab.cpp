/***************************************************************************
 * Copyright (C) 2011-2012 Fabian Lesniak <fabian.lesniak@student.kit.edu> *
 *                         Max Bruckner   <max.bruckner@student.kit.edu>   *
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

#include <QSettings>
#include <QMessageBox>

#include "qlenlab.h"
#include "signaldata.h"
#include "settingsdialog.h"
#include "exportdialog.h"
#include "plot.h"
#include "communicator.h"
#include "dockwidgets.h"
#include "storage.h"
#include "bodeplot.h"

QLenLab::QLenLab(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("QLenLab %1").arg(meta::version));
    resize(940,0);

    settingsdlg = 0;
    exportdlg = 0;
    lastbode = 0;
    fftplot = 0;

    p_storage = new storage;
    com = new communicator(p_storage,this);
    plotter = new plot(meta::scope,p_storage,this);
    settingsdlg = new settingsdialog(com,this);
    exportdlg	= new exportdialog(plotter, lastbode, this);
    label_connectionstatus = new QLabel(this);

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    tabWidget->addTab(plotter,plotter->windowTitle());
    tabWidget->setTabsClosable(true);
    QWidget *master_tab_button = tabWidget->findChild<QTabBar *>()->tabButton(0,QTabBar::RightSide);
    master_tab_button->resize(0,0);
    master_tab_button->hide();
    statusBar()->addPermanentWidget(label_connectionstatus);

    dw_scope = new dockWidget_scope(this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_scope);

    dw_trigger = new dockWidget_trigger(this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_trigger);

    dw_viewport = new dockWidget_viewport(this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_viewport);

    dw_generator = new dockWidget_generator(this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_generator);

    dw_dataview = new dockWidget_dataview(p_storage,this);
    addDockWidget(Qt::LeftDockWidgetArea,dw_dataview);
    dw_dataview->setVisible(false);

    QMenu *menu_lenlab = menuBar()->addMenu(tr("QLenLab"));
    QAction *action_settings = menu_lenlab->addAction(QIcon::fromTheme("configure"),tr("Einstellungen"));
    QAction *action_about = menu_lenlab->addAction(QIcon::fromTheme("dialog-information"),tr("Über QLenLab"));
    QAction *action_quit = menu_lenlab->addAction(QIcon::fromTheme("application-exit"),tr("Beenden"));
    QMenu *menu_interface = menuBar()->addMenu(tr("Ansicht"));
    QAction *action_scope = menu_interface->addAction(tr("Oszilloskop"));
    action_scope->setCheckable(true);
    QAction *action_viewport = menu_interface->addAction(tr("Anzeigeeinstellungen"));
    action_viewport->setCheckable(true);
    QAction *action_generator = menu_interface->addAction(tr("Frequenzgenerator"));
    action_generator->setCheckable(true);
    QAction *action_trigger = menu_interface->addAction(tr("Trigger"));
    action_trigger->setCheckable(true);
    QAction *action_dataview = menu_interface->addAction(tr("Datenspeicher"));
    action_dataview->setCheckable(true);
    QMenu *menu_measurement = menuBar()->addMenu(tr("Messung"));
    action_start = menu_measurement->addAction(tr("Starten"));
    action_stop = menu_measurement->addAction(tr("Stoppen"));
    action_bode = menu_measurement->addAction(tr("Bode-Diagramm erstellen"));
    action_fft = menu_measurement->addAction(tr("Fourier-Analyse"));
    QAction *action_export = menu_measurement->addAction(tr("Messung exportieren"));

    //various connects
    connect(com,SIGNAL(connectionStateChanged(meta::connectstate)),SLOT(setConnectionStatus(meta::connectstate)));
    connect(com,SIGNAL(measureStateChanged(bool)),SLOT(setMeasureStatus(bool)));
    connect(com,SIGNAL(displayNewDataset()),plotter,SLOT(showDataset()));
    connect(settingsdlg,SIGNAL(colorChanged(meta::colorindex,QColor)),plotter,SLOT(updateColor(meta::colorindex,QColor)));
    connect(settingsdlg,SIGNAL(colorChanged(meta::colorindex,QColor)),dw_scope,SLOT(updateColor(meta::colorindex,QColor)));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),SLOT(closeTab(int)));
    connect(tabWidget,SIGNAL(currentChanged(int)),SLOT(changedTab(int)));

    //connects for dockWidget_scope
    connect(dw_scope,SIGNAL(sampleRateChanged(unsigned int)),com,SLOT(setsamplerate(unsigned int)));
    connect(dw_scope->checkBox_ch1active,SIGNAL(toggled(bool)),com,SLOT(setchannel1active(bool)));
    connect(dw_scope->checkBox_ch2active,SIGNAL(toggled(bool)),com,SLOT(setchannel2active(bool)));
    connect(dw_scope->checkBox_ch3active,SIGNAL(toggled(bool)),com,SLOT(setchannel3active(bool)));
    connect(dw_scope->checkBox_ch4active,SIGNAL(toggled(bool)),com,SLOT(setchannel4active(bool)));
    connect(dw_scope->checkBox_ch1active,SIGNAL(toggled(bool)),dw_trigger,SLOT(channel1changed(bool)));
    connect(dw_scope->checkBox_ch2active,SIGNAL(toggled(bool)),dw_trigger,SLOT(channel2changed(bool)));
    connect(dw_scope->checkBox_ch3active,SIGNAL(toggled(bool)),dw_trigger,SLOT(channel3changed(bool)));
    connect(dw_scope->checkBox_ch4active,SIGNAL(toggled(bool)),dw_trigger,SLOT(channel4changed(bool)));
    connect(dw_scope->checkBox_ch1offset,SIGNAL(toggled(bool)),com,SLOT(setchannel1offset(bool)));
    connect(dw_scope->checkBox_ch2offset,SIGNAL(toggled(bool)),com,SLOT(setchannel2offset(bool)));
    connect(dw_scope->checkBox_ch3offset,SIGNAL(toggled(bool)),com,SLOT(setchannel3offset(bool)));
    connect(dw_scope->checkBox_ch4offset,SIGNAL(toggled(bool)),com,SLOT(setchannel4offset(bool)));
    connect(dw_scope->spinBox_ch1offset,SIGNAL(valueChanged(double)),com,SLOT(setmanualoffset1(double)));
    connect(dw_scope->spinBox_ch2offset,SIGNAL(valueChanged(double)),com,SLOT(setmanualoffset2(double)));
    connect(dw_scope->spinBox_ch3offset,SIGNAL(valueChanged(double)),com,SLOT(setmanualoffset3(double)));
    connect(dw_scope->spinBox_ch4offset,SIGNAL(valueChanged(double)),com,SLOT(setmanualoffset4(double)));
    connect(dw_scope->checkBox_ch1invert,SIGNAL(toggled(bool)),com,SLOT(setchannel1invert(bool)));
    connect(dw_scope->checkBox_ch2invert,SIGNAL(toggled(bool)),com,SLOT(setchannel2invert(bool)));
    connect(dw_scope->checkBox_ch3invert,SIGNAL(toggled(bool)),com,SLOT(setchannel3invert(bool)));
    connect(dw_scope->checkBox_ch4invert,SIGNAL(toggled(bool)),com,SLOT(setchannel4invert(bool)));
    connect(dw_scope->comboBox_range1,SIGNAL(currentIndexChanged(int)),com,SLOT(setrange1(int)));
    connect(dw_scope->comboBox_range2,SIGNAL(currentIndexChanged(int)),com,SLOT(setrange2(int)));

    //connects for dockWidget_generator
    connect(dw_generator,SIGNAL(freqSinChanged(unsigned short)),com,SLOT(setsinusfrequency(unsigned short)));
    connect(dw_generator,SIGNAL(freqSqrChanged(unsigned short)),com,SLOT(setsquarefrequency(unsigned short)));
    connect(dw_generator,SIGNAL(freqSqrRatioChanged(unsigned char)),com,SLOT(setsquareratio(unsigned char)));

    //connects for dockWidget_viewport
    connect(dw_viewport,SIGNAL(viewportXScopeChanged(int)),plotter,SLOT(updateViewportX(int)));
    connect(dw_viewport,SIGNAL(viewportYScopeChanged(double,double)),plotter,SLOT(updateViewportY(double,double)));
    connect(dw_viewport,SIGNAL(autoscaleYScopeChanged(bool)),plotter,SLOT(setYAutoscale(bool)));
    connect(dw_viewport,SIGNAL(autoscaleYScopeGridChanged(double)),plotter,SLOT(setYAutoscaleGrid(double)));
    connect(dw_viewport,SIGNAL(smoothFactorChanged(float)),com,SLOT(setsmoothfactor(float)));

    //connects for dockWidget_trigger
    connect(dw_trigger,SIGNAL(triggerModeChanged(meta::triggermode,double,unsigned char)),com,SLOT(settriggermode(meta::triggermode,double,unsigned char)));

    //connects for dockWidget_dataview
    connect(dw_dataview,SIGNAL(maximumDatasetsChanged(int)),p_storage,SLOT(setMaximumDatasets(int)));
    connect(dw_dataview,SIGNAL(showDataset(int)),plotter,SLOT(showDataset(int)));

    //connect dockWidgets and corresponding menu actions
    connect(action_viewport,SIGNAL(triggered(bool)),dw_viewport,SLOT(setShown(bool)));
    connect(action_scope,SIGNAL(triggered(bool)),dw_scope,SLOT(setShown(bool)));
    connect(action_generator,SIGNAL(triggered(bool)),dw_generator,SLOT(setShown(bool)));
    connect(action_trigger,SIGNAL(triggered(bool)),dw_trigger,SLOT(setShown(bool)));
    connect(action_dataview,SIGNAL(triggered(bool)),dw_dataview,SLOT(setShown(bool)));
    connect(dw_viewport,SIGNAL(visibilityChanged(bool)),action_viewport,SLOT(setChecked(bool)));
    connect(dw_scope,SIGNAL(visibilityChanged(bool)),action_scope,SLOT(setChecked(bool)));
    connect(dw_generator,SIGNAL(visibilityChanged(bool)),action_generator,SLOT(setChecked(bool)));
    connect(dw_trigger,SIGNAL(visibilityChanged(bool)),action_trigger,SLOT(setChecked(bool)));
    connect(dw_dataview,SIGNAL(visibilityChanged(bool)),action_dataview,SLOT(setChecked(bool)));

    //connect other menu actions
    connect(action_settings,SIGNAL(triggered()),SLOT(showSettings()));
    connect(action_about,SIGNAL(triggered()),SLOT(about()));
    connect(action_quit,SIGNAL(triggered()),SLOT(quit()));
    connect(action_start,SIGNAL(triggered()),SLOT(start()));
    connect(action_stop,SIGNAL(triggered()),SLOT(stop()));
    connect(action_bode,SIGNAL(triggered()),SLOT(initBode()));
    connect(action_fft,SIGNAL(triggered()),SLOT(showFft()));
    connect(action_export,SIGNAL(triggered()),SLOT(showExport()));

    setConnectionStatus(meta::disconnected);
    statusBar()->clearMessage();
    restoreSettings();
}

QLenLab::~QLenLab()
{
}

void QLenLab::restoreSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray(),1);
    dw_scope->restoreSettings();
    dw_viewport->restoreSettings();
    dw_generator->restoreSettings();
    dw_trigger->restoreSettings();
    dw_dataview->restoreSettings();
    settingsdlg->restoreSettings();
}

void QLenLab::closeEvent(QCloseEvent *)
{
    QSettings settings;
    settings.setValue("mainwindow/geometry",saveGeometry());
    settings.setValue("mainwindow/state",saveState(1));
    dw_scope->saveSettings();
    dw_viewport->saveSettings();
    dw_generator->saveSettings();
    dw_trigger->saveSettings();
    dw_dataview->saveSettings();
    if( fftplot != 0 )
        delete fftplot;
    delete dw_dataview;
    delete plotter;
    delete com;
    delete p_storage;
}

void QLenLab::quit()
{
    close();
}

void QLenLab::start()
{
    if( com->setRunMode(meta::measure) )
        com->start();
}

void QLenLab::stop()
{
    com->stop();
}

void QLenLab::showFft()
{
    if( fftplot == 0 ) {
        fftplot = new plot(meta::fft,p_storage,tabWidget);
        int index = tabWidget->addTab(fftplot,fftplot->windowTitle());
        fftplot->updateColor(meta::background,settingsdlg->getChannelColor(meta::background));
        fftplot->updateColor(meta::grid,settingsdlg->getChannelColor(meta::grid));
        fftplot->updateColor(meta::channel1,settingsdlg->getChannelColor(meta::channel1));
        fftplot->updateColor(meta::channel2,settingsdlg->getChannelColor(meta::channel2));
        fftplot->updateColor(meta::channel3,settingsdlg->getChannelColor(meta::channel3));
        fftplot->updateColor(meta::channel4,settingsdlg->getChannelColor(meta::channel4));
        connect(com,SIGNAL(displayNewDataset()),fftplot,SLOT(showDataset()));
        connect(settingsdlg,SIGNAL(colorChanged(meta::colorindex,QColor)),fftplot,SLOT(updateColor(meta::colorindex,QColor)));
        connect(dw_dataview,SIGNAL(showDataset(int)),fftplot,SLOT(showDataset(int)));
        connect(dw_viewport,SIGNAL(viewportXFftChanged(int)),fftplot,SLOT(updateViewportX(int)));
        connect(dw_viewport,SIGNAL(viewportYFftChanged(double,double)),fftplot,SLOT(updateViewportY(double,double)));
        connect(dw_viewport,SIGNAL(autoscaleYFftChanged(bool)),fftplot,SLOT(setYAutoscale(bool)));
        connect(dw_viewport,SIGNAL(autoscaleYFftGridChanged(double)),fftplot,SLOT(setYAutoscaleGrid(double)));
        tabWidget->setCurrentIndex(index);
        if( plotter->getCurrentIndex() != -2 ) //if plotter already display something
            fftplot->showDataset(plotter->getCurrentIndex()); //calculate and display the fft of our current picture
    } else
        tabWidget->setCurrentWidget(fftplot);
}

void QLenLab::initBode()
{
    bodeplot* bode = new bodeplot(com,this);
    lastbode = bode;
    exportdlg->setBode(lastbode);
    tabWidget->addTab(bode,bode->windowTitle());
    //tabWidget->setCurrentWidget(bode);
    bode->exec();
}

void QLenLab::showSettings()
{
    settingsdlg->exec();
}

void QLenLab::showExport()
{
    exportdlg->setBode(lastbode);
    exportdlg->exec();
}

void QLenLab::changedTab(int index)
{
    if( index < 0 )
        return;
    if ( plot* derived = dynamic_cast<plot*>(tabWidget->widget(index)) ) { //check if selected widget is a child of plot
        if( derived->getMode() == meta::fft )
            dw_viewport->setMode(meta::fft);
        else
            dw_viewport->setMode(meta::scope);
        return;
    }
    dw_viewport->setMode(meta::scope); //on bodeplot, set to scope mode
}

void QLenLab::closeTab(int index)
{
    QWidget *w = tabWidget->widget(index);
    tabWidget->removeTab(index);
    if( dynamic_cast<plot*>(w) == fftplot )
        fftplot = 0;
    delete w;
}

void QLenLab::setMeasureStatus(bool measuring)
{
    action_start->setEnabled(!measuring);
    action_stop->setEnabled(measuring);
    action_bode->setEnabled(!measuring);
}

void QLenLab::setConnectionStatus(meta::connectstate state)
{
    switch( state ) {
    case meta::connected    : label_connectionstatus->setText("<font color='green'><b>"+tr("Verbunden")+"</b></font>");
                              statusBar()->showMessage(tr("Verbunden mit: ")+com->getid(),5000);
                              action_start->setEnabled(true);
                              action_stop->setEnabled(false);
                              action_bode->setEnabled(true);
                              break;
    case meta::connectfail  : label_connectionstatus->setText("<font color='red'><b>"+tr("Nicht verbunden")+"</b></font>");
                              statusBar()->showMessage(tr("Keine Verbindung zur Karte über Schnittstelle \"")+com->port()+"\"",5000);
                              action_start->setEnabled(false);
                              action_stop->setEnabled(false);
                              action_bode->setEnabled(false);
                              break;
    case meta::disconnected : label_connectionstatus->setText("<font color='red'><b>"+tr("Nicht verbunden")+"</b></font>");
                              statusBar()->showMessage(tr("Verbindung getrennt"),5000);
                              action_start->setEnabled(false);
                              action_stop->setEnabled(false);
                              action_bode->setEnabled(false);
                              break;
    case meta::connecting   : label_connectionstatus->setText("<font color='red'><b>"+tr("Nicht verbunden")+"</b></font>");
                              statusBar()->showMessage(tr("Verbindungsaufbau über Schnittstelle \"")+com->port()+"\"",5000);
                              action_start->setEnabled(false);
                              action_stop->setEnabled(false);
                              action_bode->setEnabled(false);
                              break;
    }
}

void QLenLab::about()
{
    QMessageBox::information(this,tr("Über QLenLab"),
                             tr("QLenLab ist ein Softwareprojekt, das die Ansteuerung der Hardware-Platine LENlab des Instituts für Biomedizinische Technik (IBT, http://ibt.kit.edu) "
                                "am Karlsruhe Institute of Technology (KIT) "
                                "ermöglichen soll.\n\nEs steht in keinerlei Zusammenhang mit den Entwicklern dieser Platform. Die Nutzung von QLenLab erfolgt auf eigene Gefahr, "
                                "allgemein gilt:\n")+QString("\nQLenLab is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by "
                                "the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n"
                                "QLenLab is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
                                "See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with QLenLab. "
                                "If not, see <http://www.gnu.org/licenses/>.\n\nQLenLab is based in part on the work of the Qwt project (http://qwt.sf.net)."
                                "\n\nQLenLab is based in part on the work of the LENlib project.\n\nQLenLab uses libfftw3 (http://www.fftw.org/)"));
}
