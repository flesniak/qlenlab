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

#include <QtGui>
#include <qwt6/qwt_plot_grid.h>
#include <qwt6/qwt_plot_curve.h>
#include <qwt6/qwt_plot_canvas.h>
#include <qwt6/qwt_plot_layout.h>
#include <qwt6/qwt_scale_engine.h>

#include "bodeplot.h"
#include "signaldata.h"
#include "communicator.h"

bodeplot::bodeplot(communicator* com, QWidget *parent) : QwtPlot(parent), p_com(com)
{
    setWindowTitle(tr("Bode-Diagramm"));
    setAxisTitle(QwtPlot::xBottom, tr("Frequenz [Hz]"));
    setAxisTitle(QwtPlot::yLeft, tr("Verstärkung [dB]"));
    plotLayout()->setAlignCanvasToScales(true);

    QwtPlotGrid* grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

    setAxisScaleEngine(QwtPlot::xBottom, new QwtLog10ScaleEngine);

    p_data = new bodedata;

    QwtPlotCurve* curve = new QwtPlotCurve(tr("Amplitudengang"));
    curve->setData(p_data);
    curve->attach(this);

    grid->setPen(QPen(Qt::white, 0.0, Qt::DotLine));
    curve->setPen(QPen(QColor(255,255,255)));
    setCanvasBackground(QColor(Qt::black));
}

bool bodeplot::exec()
{
    bodedialog *bd = new bodedialog(this);
    bool state = bd->exec();
    setAxisScale(QwtPlot::xBottom, p_data->boundingRect().left(), p_data->boundingRect().right());
    setAxisScale(QwtPlot::yLeft,  p_data->boundingRect().bottom(),  p_data->boundingRect().top());
    replot();
    delete bd;
    return state;
}

bodedialog::bodedialog(bodeplot *parent) : QDialog(parent), parent(parent)
{
    setWindowTitle("Bode-Diagramm messen");

    groupBox_parameters = new QGroupBox(this);
    spinBox_freqStart = new QSpinBox(groupBox_parameters);
    spinBox_freqStart->setRange(1,9999);
    spinBox_freqStart->setValue(10);
    spinBox_freqEnd = new QSpinBox(groupBox_parameters);
    spinBox_freqEnd->setRange(2,10000);
    spinBox_freqEnd->setValue(10000);
    spinBox_decadePoints = new QSpinBox(groupBox_parameters);
    spinBox_decadePoints->setRange(1,30);
    spinBox_decadePoints->setValue(10);
    QFormLayout *groupBox_parameters_layout = new QFormLayout(groupBox_parameters);
    groupBox_parameters_layout->addRow(tr("Startfrequenz"),spinBox_freqStart);
    groupBox_parameters_layout->addRow(tr("Endfrequenz"),spinBox_freqEnd);
    groupBox_parameters_layout->addRow(tr("Punkte pro Dekade"),spinBox_decadePoints);

    groupBox_inputButtons = new QGroupBox(this);
    groupBox_inputButtons->setTitle(tr("Eingang"));
    radioButton_inputch1 = new QRadioButton(tr("Kanal 1"),groupBox_inputButtons);
    radioButton_inputch2 = new QRadioButton(tr("Kanal 2"),groupBox_inputButtons);
    radioButton_inputch3 = new QRadioButton(tr("Kanal 3"),groupBox_inputButtons);
    radioButton_inputch4 = new QRadioButton(tr("Kanal 4"),groupBox_inputButtons);
    QVBoxLayout *groupBox_inputButtons_layout = new QVBoxLayout(groupBox_inputButtons);
    groupBox_inputButtons_layout->addWidget(radioButton_inputch1);
    groupBox_inputButtons_layout->addWidget(radioButton_inputch2);
    groupBox_inputButtons_layout->addWidget(radioButton_inputch3);
    groupBox_inputButtons_layout->addWidget(radioButton_inputch4);
    radioButton_inputch1->setChecked(true);

    groupBox_outputButtons = new QGroupBox(this);
    groupBox_outputButtons->setTitle(tr("Ausgang"));
    radioButton_outputch1 = new QRadioButton(tr("Kanal 1"),groupBox_inputButtons);
    radioButton_outputch2 = new QRadioButton(tr("Kanal 2"),groupBox_inputButtons);
    radioButton_outputch3 = new QRadioButton(tr("Kanal 3"),groupBox_inputButtons);
    radioButton_outputch4 = new QRadioButton(tr("Kanal 4"),groupBox_inputButtons);
    QVBoxLayout *groupBox_outputButtons_layout = new QVBoxLayout(groupBox_outputButtons);
    groupBox_outputButtons_layout->addWidget(radioButton_outputch1);
    groupBox_outputButtons_layout->addWidget(radioButton_outputch2);
    groupBox_outputButtons_layout->addWidget(radioButton_outputch3);
    groupBox_outputButtons_layout->addWidget(radioButton_outputch4);
    radioButton_outputch3->setChecked(true);

    groupBox_bodeStatus = new QGroupBox(this);
    label_frequency = new QLabel("0.0",groupBox_bodeStatus);
    label_inputAmplitude = new QLabel("0.0",groupBox_bodeStatus);
    label_outputAmplitude = new QLabel("0.0",groupBox_bodeStatus);
    label_amplification = new QLabel("0.0",groupBox_bodeStatus);
    label_samplerate = new QLabel("0.0",groupBox_bodeStatus);
    QFormLayout *groupBox_bodeStatus_layout = new QFormLayout(groupBox_bodeStatus);
    groupBox_bodeStatus_layout->addRow(tr("Frequenz"),label_frequency);
    groupBox_bodeStatus_layout->addRow(tr("Eingangsamplitude"),label_inputAmplitude);
    groupBox_bodeStatus_layout->addRow(tr("Ausgangsamplitude"),label_outputAmplitude);
    groupBox_bodeStatus_layout->addRow(tr("Verstärkung"),label_amplification);
    groupBox_bodeStatus_layout->addRow(tr("Samplerate"),label_samplerate);
    groupBox_bodeStatus->setEnabled(false);

    progressBar_progress = new QProgressBar(this);
    progressBar_progress->setRange(0,100);
    progressBar_progress->setValue(0);
    progressBar_progress->setEnabled(false);

    pushButton_start = new QPushButton(tr("Start"),this);
    pushButton_cancel = new QPushButton(tr("Abbrechen"),this);

    QHBoxLayout *layout_groups = new QHBoxLayout;
    layout_groups->addWidget(groupBox_parameters);
    layout_groups->addWidget(groupBox_inputButtons);
    layout_groups->addWidget(groupBox_outputButtons);
    layout_groups->addWidget(groupBox_bodeStatus);

    QHBoxLayout *layout_bottom = new QHBoxLayout;
    layout_bottom->addWidget(progressBar_progress);
    layout_bottom->addWidget(pushButton_start);
    layout_bottom->addWidget(pushButton_cancel);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(layout_groups);
    layout->addLayout(layout_bottom);

    connect(pushButton_start,SIGNAL(clicked()),SLOT(start()));
    connect(pushButton_cancel,SIGNAL(clicked()),SLOT(cancel()));

    connect(radioButton_inputch1,SIGNAL(toggled(bool)),SLOT(inputRadiosChanged()));
    connect(radioButton_inputch2,SIGNAL(toggled(bool)),SLOT(inputRadiosChanged()));
    connect(radioButton_inputch3,SIGNAL(toggled(bool)),SLOT(inputRadiosChanged()));
    connect(radioButton_inputch4,SIGNAL(toggled(bool)),SLOT(inputRadiosChanged()));
    connect(radioButton_outputch1,SIGNAL(toggled(bool)),SLOT(outputRadiosChanged()));
    connect(radioButton_outputch2,SIGNAL(toggled(bool)),SLOT(outputRadiosChanged()));
    connect(radioButton_outputch3,SIGNAL(toggled(bool)),SLOT(outputRadiosChanged()));
    connect(radioButton_outputch4,SIGNAL(toggled(bool)),SLOT(outputRadiosChanged()));
}

void bodedialog::closeEvent(QCloseEvent *)
{
    if( parent->p_com->isRunning() )
        parent->p_com->stop();
}

void bodedialog::start()
{
    if( parent->p_com->setRunMode(meta::bode) ) {
        groupBox_parameters->setEnabled(false);
        groupBox_inputButtons->setEnabled(false);
        groupBox_outputButtons->setEnabled(false);
        groupBox_bodeStatus->setEnabled(true);
        progressBar_progress->setEnabled(true);
        pushButton_start->setEnabled(false);
        connect(parent->p_com,SIGNAL(bodeStateUpdate(bodestate)),SLOT(updateStatus(bodestate)));
        connect(parent->p_com,SIGNAL(measureStateChanged(bool)),SLOT(finished(bool)));
        parent->p_com->setBodeParameters(parent->p_data,spinBox_freqStart->value(),spinBox_freqEnd->value(),spinBox_decadePoints->value(),selectedInput(),selectedOutput());
        qDebug() << "[bodeplot] Committing com start";
        parent->p_com->start();
    }
}

void bodedialog::finished(bool stillmeasuring)
{
    if( !stillmeasuring ) {
        groupBox_parameters->setEnabled(true);
        groupBox_inputButtons->setEnabled(true);
        groupBox_outputButtons->setEnabled(true);
        groupBox_bodeStatus->setEnabled(false);
        progressBar_progress->setEnabled(false);
        pushButton_start->setEnabled(true);
        disconnect(parent->p_com,SIGNAL(bodeStateUpdate(bodestate)),this,SLOT(updateStatus(bodestate)));
        disconnect(parent->p_com,SIGNAL(measureStateChanged(bool)),this,SLOT(finished(bool)));
        accept();
    }
}

void bodedialog::cancel()
{
    if( parent->p_com->isRunning() )
        parent->p_com->stop();
    else
        reject();
}

meta::channel bodedialog::selectedInput()
{
    if( radioButton_inputch1->isChecked() )
        return meta::ch1a;
    if( radioButton_inputch2->isChecked() )
        return meta::ch1b;
    if( radioButton_inputch3->isChecked() )
        return meta::ch2a;
    if( radioButton_inputch4->isChecked() )
        return meta::ch2b;
    return meta::ch12ab;
}

meta::channel bodedialog::selectedOutput()
{
    if( radioButton_outputch1->isChecked() )
        return meta::ch1a;
    if( radioButton_outputch2->isChecked() )
        return meta::ch1b;
    if( radioButton_outputch3->isChecked() )
        return meta::ch2a;
    if( radioButton_outputch4->isChecked() )
        return meta::ch2b;
    return meta::ch12ab;
}

void bodedialog::updateStatus(bodestate state)
{
    label_frequency->setText(QString::number(state.frequency)+"Hz");
    label_inputAmplitude->setText(QString::number(state.inputAmplitude,'g',2)+"V");
    label_outputAmplitude->setText(QString::number(state.outputAmplitude,'g',2)+"V");
    label_amplification->setText(QString::number(state.amplification,'g',2)+"dB");
    label_samplerate->setText(QString::number(state.samplerate)+"Hz");
    progressBar_progress->setValue(state.progress);
}

void bodedialog::inputRadiosChanged()
{
    if( (radioButton_inputch1->isChecked() || radioButton_inputch2->isChecked() ) && (radioButton_outputch1->isChecked() || radioButton_outputch2->isChecked() ) )
        radioButton_outputch3->setChecked(true);
    if( (radioButton_inputch3->isChecked() || radioButton_inputch4->isChecked() ) && (radioButton_outputch3->isChecked() || radioButton_outputch4->isChecked() ) )
        radioButton_outputch1->setChecked(true);
}

void bodedialog::outputRadiosChanged()
{
    if( (radioButton_inputch1->isChecked() || radioButton_inputch2->isChecked() ) && (radioButton_outputch1->isChecked() || radioButton_outputch2->isChecked() ) )
        radioButton_inputch3->setChecked(true);
    if( (radioButton_inputch3->isChecked() || radioButton_inputch4->isChecked() ) && (radioButton_outputch3->isChecked() || radioButton_outputch4->isChecked() ) )
        radioButton_inputch1->setChecked(true);
}
