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

QLenLab::QLenLab(QWidget *parent) : QMainWindow(parent), ui(new Ui::QLenLab)
{
    ui->setupUi(this);

    debug = NULL;
    settingsdlg = NULL;

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    plot = new Plot(this);

    tabWidget->addTab(plot,tr("Plot"));

    label_connectionstatus = new QLabel(this);
    setConnectionStatus(false);
    ui->statusBar->addWidget(label_connectionstatus);

    ui->action_quit->setIcon(QIcon::fromTheme("application-exit"));

    connect(ui->comboBox_xaxis,SIGNAL(currentIndexChanged(QString)),SLOT(viewportXChanged(QString)));
    connect(ui->doubleSpinBox_yaxis_lower,SIGNAL(valueChanged(double)),SLOT(viewportYChanged()));
    connect(ui->doubleSpinBox_yaxis_upper,SIGNAL(valueChanged(double)),SLOT(viewportYChanged()));
    connect(ui->action_viewport,SIGNAL(triggered(bool)),ui->dockWidget_viewport,SLOT(setShown(bool)));
    connect(ui->action_scope,SIGNAL(triggered(bool)),ui->dockWidget_scope,SLOT(setShown(bool)));
    connect(ui->dockWidget_viewport,SIGNAL(visibilityChanged(bool)),ui->action_viewport,SLOT(setChecked(bool)));
    connect(ui->dockWidget_scope,SIGNAL(visibilityChanged(bool)),ui->action_scope,SLOT(setChecked(bool)));
    connect(ui->action_debug,SIGNAL(triggered()),SLOT(showDebug()));
    connect(ui->action_settings,SIGNAL(triggered()),SLOT(showSettings()));
    connect(ui->action_about,SIGNAL(triggered()),SLOT(about()));
    connect(ui->action_quit,SIGNAL(triggered()),SLOT(quit()));

    restoreSettings();

    ui->action_viewport->setChecked(ui->dockWidget_viewport->isVisible());
    ui->action_scope->setChecked(ui->dockWidget_scope->isVisible());
    plot->updateViewportX(ui->comboBox_xaxis->currentText().toInt());
    plot->updateViewportY(ui->doubleSpinBox_yaxis_lower->value(),ui->doubleSpinBox_yaxis_upper->value());

    QwtPlotCurve curve1;
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
    close();
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
        label_connectionstatus->setText(tr("<font color='green'><b>Verbunden</b></font>"));
    else
        label_connectionstatus->setText(tr("<font color='red'><b>Nicht verbunden</b></font>"));
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
