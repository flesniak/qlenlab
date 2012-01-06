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

#ifndef QLENLAB_H
#define QLENLAB_H

#include <QMainWindow>
#include <QLabel>
#include <QTabWidget>
#include <QString>
#include <QMutex>

class settingsdialog;
class plot;
class communicator;
class signaldata;
class dockWidget_scope;
class dockWidget_viewport;
class dockWidget_generator;

class QLenLab : public QMainWindow
{
    Q_OBJECT

public:
    explicit QLenLab(QWidget *parent = 0);
    ~QLenLab();

public slots:
    void setConnectionStatus(bool connected);

protected:
    void closeEvent(QCloseEvent *);

private:
    void restoreSettings();

    dockWidget_scope *dw_scope;
    dockWidget_viewport *dw_viewport;
    dockWidget_generator *dw_generator;

    QLabel *label_connectionstatus;
    QTabWidget *tabWidget;
    QAction *action_start;
    QAction *action_stop;
    settingsdialog *settingsdlg;
    plot *plotter;
    communicator *com;
    signaldata *data;

private slots:
    void start();
    void stop();
    void showSettings();
    void about();
    void quit();
};

#endif // QLENLAB_H
