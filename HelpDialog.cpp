/*
 * This file is part of QRssReader, rss reader for pc and android.
 * Copyright (C) 2014, Petr Kačer <kacerpetr@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QPainter>
#include <QFile>
#include <QScroller>
#include "HelpDialog.h"
#include "ui_HelpDialog.h"

/**
 * @brief Class constructor
 * @param parent
 */
HelpDialog::HelpDialog(QWidget* parent) : QDialog(parent), ui(new Ui::HelpDialog){
    ui->setupUi(this);

    //enables figer scrolling for android
    #ifdef ANDROID
        QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
        QScrollerProperties sp;
        sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
        sp.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
        sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.1);
        sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        sp.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        sp.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.1);
        sp.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
        QScroller* qs = QScroller::scroller(ui->scrollArea);
        qs->setScrollerProperties(sp);
    #endif

    //loads help text from resources
    #ifdef ANDROID
        QFile file(":/strings/help_android");
        file.open(QIODevice::ReadOnly);
        ui->mainLabel->setText(file.readAll());
        file.close();
    #else
        QFile file(":/strings/help_pc");
        file.open(QIODevice::ReadOnly);
        ui->mainLabel->setText(file.readAll());
        file.close();
    #endif
}

/**
 * @brief Class destructor
 */
HelpDialog::~HelpDialog(){
    delete ui;
}
