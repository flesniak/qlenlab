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

#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QDialog>
#include <QMessageBox>

#include <QDockWidget>

class dockWidget_test : public QDockWidget
{
public:
    dockWidget_test(QWidget *parent = 0);
};

namespace Ui {
    class debugger;
}

class debugger : public QDialog
{
    Q_OBJECT

public:
    explicit debugger(QWidget *parent = 0);
    ~debugger();
    bool openport(char*);

public slots:
    void processIncoming(QByteArray data);

private:
    Ui::debugger *ui;

private slots:
    void updateEditfields(int index);
    void sendCommand();
};

#endif // DEBUGGER_H
