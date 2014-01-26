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

#include "NewsGroupWidget.h"
#include "ui_NewsGroupWidget.h"
#include <QPainter>

/**
 * @brief Class constructor
 * @param parent
 */
NewsGroupWidget::NewsGroupWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NewsGroupWidget){
    ui->setupUi(this);
}

/**
 * @brief Given text will be shown
 * @param text usualy serves as header of day marker
 */
void NewsGroupWidget::setText(QString text){
    ui->textLabel->setText(text);
}

/**
 * @brief Because of using stylesheets
 */
void NewsGroupWidget::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/**
 * @brief Class destructor
 */
NewsGroupWidget::~NewsGroupWidget(){
    delete ui;
}

