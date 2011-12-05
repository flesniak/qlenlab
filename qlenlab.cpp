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

#include "qlenlab.h"
#include "ui_qlenlab.h"

QLenLab::QLenLab(QWidget *parent) : QMainWindow(parent), ui(new Ui::QLenLab)
{
    debug = NULL;
    settingsdlg = NULL;

    ui->setupUi(this);

    label_connectionstatus = new QLabel(this);
    setConnectionStatus(false);
    ui->statusBar->addWidget(label_connectionstatus);

    ui->action_quit->setIcon(QIcon::fromTheme("application-exit"));

    connect(ui->action_viewport,SIGNAL(triggered(bool)),ui->dockWidget_viewport,SLOT(setShown(bool)));
    connect(ui->action_scope,SIGNAL(triggered(bool)),ui->dockWidget_scope,SLOT(setShown(bool)));
    connect(ui->dockWidget_viewport,SIGNAL(visibilityChanged(bool)),ui->action_viewport,SLOT(setChecked(bool)));
    connect(ui->dockWidget_scope,SIGNAL(visibilityChanged(bool)),ui->action_scope,SLOT(setChecked(bool)));

    connect(ui->action_debug,SIGNAL(triggered()),SLOT(showDebug()));
    connect(ui->action_settings,SIGNAL(triggered()),SLOT(showSettings()));
    connect(ui->action_quit,SIGNAL(triggered()),SLOT(quit()));

    QSettings settings;
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    restoreState(settings.value("mainwindow/state").toByteArray(),1);

    ui->action_viewport->setChecked(ui->dockWidget_viewport->isVisible());
    ui->action_scope->setChecked(ui->dockWidget_scope->isVisible());
}

QLenLab::~QLenLab()
{
    delete ui;
}

void QLenLab::closeEvent(QCloseEvent *)
{
    QSettings settings;
    settings.setValue("mainwindow/geometry",saveGeometry());
    settings.setValue("mainwindow/state",saveState(1));
}

void QLenLab::quit()
{
    close();
}

void QLenLab::showSettings()
{
    if( settingsdlg == NULL )
        settingsdlg = new settingsdialog(this);
    settingsdlg->exec();
}

void QLenLab::showDebug()
{
    if( debug == NULL )
        debug = new debugger(this);
    debug->exec();
}

void QLenLab::setConnectionStatus(bool connected)
{
    if( connected )
        label_connectionstatus->setText("<font color='green'><b>Verbunden</b></font>");
    else
        label_connectionstatus->setText("<font color='red'><b>Nicht verbunden</b></font>");
}
