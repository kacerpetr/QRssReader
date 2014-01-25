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

#include "NewsItemWidget.h"
#include "ui_NewsItemWidget.h"
#include <QPainter>
#include <QDebug>
#include "SettingsModel.h"

NewsItemWidget::NewsItemWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NewsItemWidget), selected(false){
    ui->setupUi(this);

    //settings model instance
    SettingsModel& sm = SettingsModel::get();

    //data changed signal
    connect(&sm, SIGNAL(dataChanged(QString)), this, SLOT(settingsChanged(QString)));

    //set fonts
    settingsChanged("list_title_font_size");
    settingsChanged("list_time_font_size");
}

void NewsItemWidget::settingsChanged(QString key){
    //changes title size
    if(key == "list_title_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont titleFont;
        titleFont.setBold(true);
        titleFont.setPointSize(sm.getInt(key));
        ui->titleLabel->setFont(titleFont);
    }
    //changes time - feed label font size
    else if(key == "list_time_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont timeFont;
        timeFont.setItalic(true);
        timeFont.setPointSize(sm.getInt(key));
        ui->feedLabel->setFont(timeFont);
    }
}

void NewsItemWidget::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NewsItemWidget::setNewsItem(NewsItem item){
    //set labels
    ui->titleLabel->setText(item.header);
    ui->feedLabel->setText(item.time.time().toString("hh:mm:ss") + " - " + item.feed.name);

    //set colors
    setColor(item.feed.bkgColor, item.feed.textColor);

    //stores news item
    this->item = item;
}

void NewsItemWidget::setSelected(bool selected){
    if(selected){
        QColor bkgClr = item.feed.bkgColor;
        QString clrStr = QString::number((bkgClr.red()<=40) ? 0 : (bkgClr.red()-40)) + ",";
        clrStr += QString::number((bkgClr.green()<=40) ? 0 : (bkgClr.green()-40)) + ",";
        clrStr += QString::number((bkgClr.blue()<=40) ? 0 : (bkgClr.blue()-40));
        ui->selectHighlight->setStyleSheet("background-color: rgb(" + clrStr + ")");
    }else{
        ui->selectHighlight->setStyleSheet("");
    }
    this->selected = selected;
}

NewsItem* NewsItemWidget::newsItem(){
    return &item;
}

bool NewsItemWidget::isSelected() const{
    return selected;
}

void NewsItemWidget::setColor(QColor bkgColor, QColor textColor){
    //background color
    QString textClrStr = QString::number(textColor.red()) + ",";
    textClrStr += QString::number(textColor.green()) + ",";
    textClrStr += QString::number(textColor.blue());

    //text color
    QString bkgClrStr = QString::number(bkgColor.red()) + ",";
    bkgClrStr += QString::number(bkgColor.green()) + ",";
    bkgClrStr += QString::number(bkgColor.blue());

    //text color
    QString styleSheet = QString(
        "#NewsItemWidget{ \n"
        "    background-color: rgb(" + bkgClrStr + "); \n"
        "    color: rgb(" + textClrStr + "); \n"
        "} \n"
    );

    //sets stylesheet
    setStyleSheet(styleSheet);
}

void NewsItemWidget::mousePressEvent(QMouseEvent*){
    emit pressed(this);
}

NewsItemWidget::~NewsItemWidget(){
    delete ui;
}

