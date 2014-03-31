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
#include <QDebug>
#include <QFontMetrics>
#include <QResizeEvent>
#include "NewsItemWidget.h"
#include "ui_NewsItemWidget.h"
#include "SettingsModel.h"

/**
 * @brief Class constructor
 * @param parent
 */
NewsItemWidget::NewsItemWidget(QWidget* parent) : QWidget(parent),
ui(new Ui::NewsItemWidget), selected(false){
    ui->setupUi(this);

    //settings model instance
    SettingsModel& sm = SettingsModel::get();

    //data changed signal
    connect(&sm, SIGNAL(dataChanged(QString)), this, SLOT(settingsChanged(QString)));

    //set fonts
    settingsChanged("list_title_font_size");
    settingsChanged("list_time_font_size");
}

/**
 * @brief Class destructor
 */
NewsItemWidget::~NewsItemWidget(){
    delete ui;
}

/**
 * @brief Called when settings was changed
 * @param tag key of item in settings
 */
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

/**
 * @brief Because of using stylesheets
 */
void NewsItemWidget::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/**
 * @brief Shows title and other from given NewsItem
 * @param item this one will be shown in news list
 */
void NewsItemWidget::setNewsItem(TRssItem item){
    //set labels
    ui->titleLabel->setText(item.title);
    ui->feedLabel->setText(item.dt.time().toString("hh:mm:ss") + " - " + item.channel.title);

    //stores news item
    this->item = item;
}

/**
 * @brief Selects or unselects this item
 * @param selected true to select, false to unselect
 */
void NewsItemWidget::setSelected(bool selected){
    if(selected){
        ui->selectionHighlight->setStyleSheet(
            "#selectionHighlight{\n"
            "   background-image: url(:/images/selectionHighlight);\n"
            "}"
        );
    }else{
        ui->selectionHighlight->setStyleSheet("");
    }
    this->selected = selected;
}

/**
 * @brief Sets item background color
 * @param true when item is odd in news list
 */
void NewsItemWidget::setOdd(bool odd){
    //first background color
    if(odd){
        setStyleSheet(
            "#NewsItemWidget{"
            "   background-color: rgb(240,240,240);"
            "}"
        );
    }
    //second background color
    else{
        setStyleSheet(
            "#NewsItemWidget{"
            "   background-color: rgb(225,225,225);"
            "}"
        );
    }
}

/**
 * @brief Returns pointer of asociated NewsItem
 */
TRssItem* NewsItemWidget::newsItem(){
    return &item;
}

/**
 * @brief Returns true if item is selected
 */
bool NewsItemWidget::isSelected() const{
    return selected;
}

/**
 * @brief Sets color and number of feed icon
 * @param background background color
 * @param index number viewed in feed icon
 */
void NewsItemWidget::setIcon(QColor background, int number){
    //background color
    QString textClrStr = QString::number(background.red()) + ",";
    textClrStr += QString::number(background.green()) + ",";
    textClrStr += QString::number(background.blue());

    //sets icon stylesheet
    ui->feedIconLabel->setStyleSheet(
        "#feedIconLabel{\n"
        "   border-radius: 17px;\n"
        "   color: white;\n"
        "   font-weight: bold;\n"
        "   background: rgb("+textClrStr+");\n"
        "}"
    );

    //feed index
    ui->feedIconLabel->setText(QString::number(number));
}

/**
 * Emits pressed signal on mouse press event
 */
void NewsItemWidget::mousePressEvent(QMouseEvent*){
    emit pressed(this);
}
