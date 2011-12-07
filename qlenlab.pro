# ************************************************************************
# * Copyright (C) 2011 Fabian Lesniak <fabian.lesniak@student.kit.edu>   *
# *                                                                      *
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
LIBS += -lstdc++

SOURCES += main.cpp qlenlab.cpp \
    debugger.cpp \
    settingsdialog.cpp \
    plot.cpp \
    signaldata.cpp \
    LENlib/LENlib.cpp \
    communicator.cpp
HEADERS += qlenlab.h \
    debugger.h \
    settingsdialog.h \
    plot.h \
    signaldata.h \
    LENlib/LENlib.h \
    communicator.h
FORMS += qlenlab.ui \
    debugger.ui \
    settingsdialog.ui




