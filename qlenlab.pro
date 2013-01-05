# ************************************************************************
# * Copyright (C) 2011 Fabian Lesniak <fabian.lesniak@student.kit.edu>   *
# *                    Max Bruckner   <max.bruckner@student.kit.edu>     *
# * This file is part of the QLenLab project.                            *
# *                                                                      *
# * QLenLab is free software: you can redistribute it and/or modify      *
# * it under the terms of the GNU General Public License as published by *
# * the Free Software Foundation, either version 3 of the License, or    *
# * (at your option) any later version.                                  *
# *                                                                      *
# * QLenLab is distributed in the hope that it will be useful,           *
# * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         *
# * GNU General Public License for more details.                         *
# *                                                                      *
# * You should have received a copy of the GNU General Public License    *
# * along with QLenLab. If not, see <http://www.gnu.org/licenses/>.      *
# ************************************************************************

QT += core gui
TARGET = qlenlab
CONFIG += qwt

#If your installation of qwt does not install its feature file correctly,
#uncomment and edit the following lines to match your installation.
#For further information, see: http://qwt.sourceforge.net/qwtinstall.html
#LIBS += -lstdc++ -lqwt
#INCLUDEPATH += /usr/include/qwt6

SOURCES += main.cpp qlenlab.cpp settingsdialog.cpp exportdialog.cpp plot.cpp signaldata.cpp LENlib/LENlib.cpp communicator.cpp dockwidgets.cpp storage.cpp bodeplot.cpp
HEADERS += qlenlab.h settingsdialog.h exportdialog.h plot.h signaldata.h LENlib/LENlib.h communicator.h meta.h dockwidgets.h storage.h bodeplot.h
FORMS +=

