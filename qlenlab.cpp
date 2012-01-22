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

#include <QSettings>
#include <QMessageBox>

#include "qlenlab.h"
#include "signaldata.h"
#include "settingsdialog.h"
#include "plot.h"
#include "communicator.h"
#include "dockwidgets.h"
#include "storage.h"

QLenLab::QLenLab(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("QLenLab %1").arg(meta::version));
    resize(940,0);

    settingsdlg = NULL;

    p_storage = new storage;
    com = new communicator(p_storage,this);
    plotter = new plot(p_storage,this);
    settingsdlg = new settingsdialog(com,this);

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
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

    label_connectionstatus = new QLabel(this);
    tabWidget->addTab(plotter,tr("Plot"));
    statusBar()->addPermanentWidget(label_connectionstatus);

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

    //various connects
    connect(com,SIGNAL(connectionStateChanged(bool)),SLOT(setConnectionStatus(bool)));
    connect(com,SIGNAL(measureStateChanged(bool)),SLOT(setMeasureStatus(bool)));
    connect(com,SIGNAL(newDatasetComplete()),plotter,SLOT(showDataset()));
    connect(settingsdlg,SIGNAL(colorChanged(meta::colorindex,QColor)),plotter,SLOT(updateColor(meta::colorindex,QColor)));
    connect(settingsdlg,SIGNAL(colorChanged(meta::colorindex,QColor)),dw_scope,SLOT(updateColor(meta::colorindex,QColor)));

    //connects for dockWidget_scope
    connect(dw_scope,SIGNAL(sampleRateChanged(unsigned int)),com,SLOT(setsamplerate(unsigned int)));
    connect(dw_scope->checkBox_ch1active,SIGNAL(toggled(bool)),com,SLOT(setchannel1active(bool)));
    connect(dw_scope->checkBox_ch2active,SIGNAL(toggled(bool)),com,SLOT(setchannel2active(bool)));
    connect(dw_scope->checkBox_ch3active,SIGNAL(toggled(bool)),com,SLOT(setchannel3active(bool)));
    connect(dw_scope->checkBox_ch4active,SIGNAL(toggled(bool)),com,SLOT(setchannel4active(bool)));
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
    connect(dw_viewport,SIGNAL(viewportXChanged(int)),plotter,SLOT(updateViewportX(int)));
    connect(dw_viewport,SIGNAL(viewportYChanged(double,double)),plotter,SLOT(updateViewportY(double,double)));
    connect(dw_viewport,SIGNAL(autoscaleYChanged(bool)),plotter,SLOT(setYAutoscale(bool)));

    //connects for dockWidget_trigger
    connect(dw_trigger,SIGNAL(triggerModeChanged(meta::triggermode,double)),com,SLOT(settriggermode(meta::triggermode,double)));

    //connects for dockWidget_dataview
    connect(dw_dataview,SIGNAL(maximumDatasetsChanged(int)),p_storage,SLOT(setMaximumDatasets(int)));
    connect(dw_dataview,SIGNAL(showDataset(int)),plotter,SLOT(showDataset(int)));

    //connect DockWidgets and corresponding menu actions
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

    setConnectionStatus(false);
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
    com->start();
}

void QLenLab::stop()
{
    com->stop();
}

void QLenLab::showSettings()
{
    settingsdlg->exec();
}

void QLenLab::setMeasureStatus(bool measuring)
{
    action_start->setEnabled(!measuring);
    action_stop->setEnabled(measuring);
}

void QLenLab::setConnectionStatus(bool connected)
{
    if( connected ) {
        label_connectionstatus->setText("<font color='green'><b>"+tr("Verbunden")+"</b></font>");
        statusBar()->showMessage(tr("Verbunden mit: ")+com->getid(),5000);
        action_start->setEnabled(true);
        action_stop->setEnabled(false);
    }
    else {
        label_connectionstatus->setText("<font color='red'><b>"+tr("Nicht verbunden")+"</b></font>");
        statusBar()->showMessage(tr("Keine Verbindung zur Karte über Schnittstelle \"")+com->lastTriedPort()+"\"",5000);
        action_start->setEnabled(false);
        action_stop->setEnabled(false);
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
                                "\n\nQLenLab is based in part on the work of the LENlib project."));
}
