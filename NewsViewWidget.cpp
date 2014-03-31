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

#include <QScroller>
#include "NewsViewWidget.h"
#include "ui_NewsViewWidget.h"
#include "SettingsModel.h"

/**
 * @brief Class constructor
 */
NewsViewWidget::NewsViewWidget(QWidget *parent) : QWidget(parent), ui(new Ui::NewsViewWidget){
    ui->setupUi(this);

    //finger (left mouse) scrolling
    QScroller::grabGesture(ui->newsView, QScroller::LeftMouseButtonGesture);
    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    sp.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.1);
    sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    sp.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    sp.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.1);
    sp.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
    QScroller* qs = QScroller::scroller(ui->newsView);
    qs->setScrollerProperties(sp);

    //settings change handling
    connect(&SettingsModel::get(), SIGNAL(dataChanged(QString)), this, SLOT(settingsChanged(QString)));
}

/**
 * @brief Class destructor
 */
NewsViewWidget::~NewsViewWidget(){
    delete ui;
}

/**
 * @brief Called when item from news list is pressed
 * @param item pointer to selected news item
 */
void NewsViewWidget::itemPressed(TRssItem* item){
    //feed name and description
    QString name = "Feed: " + item->feed.name;
    if(!item->feed.desc.isEmpty())
        name += " - " + (item->channel.desc.isEmpty() ? item->feed.desc : item->channel.desc);
    ui->feedNameLabel->setText(name);

    //report time
    ui->timeLabel->setText("Datetime: " +
    item->dt.toString("ddd, dd MMM yyyy hh:mm:ss"));

    //link of full report
    QString linkText = item->link;
    if(item->link.length() > 50) linkText = item->link.left(20) + " ... " + item->link.right(20);
    ui->linkLabel->setText("Link: <a href=\""+item->link+"\">" + linkText + "</a>");

    //other report parts
    ui->titleLabel->setText(item->title);
    ui->textLabel->setText(item->desc.trimmed());
    ui->guidLabel->setText("Guid: " + item->guid);

    //image label background color
    QString textClrStr = QString::number(item->feed.bkgColor.red()) + ",";
    textClrStr += QString::number(item->feed.bkgColor.green()) + ",";
    textClrStr += QString::number(item->feed.bkgColor.blue());
    textClrStr = "background: rgb(" + textClrStr + ");";
    ui->chDesc->setStyleSheet(textClrStr);
    ui->chTitle->setStyleSheet(textClrStr);

    #ifdef ANDROID
    //channel title
    ui->chTitle->setText(
        "<html><head/><body><p>"
        "<span style=\"font-size:54pt; font-weight:200; color:rgba(255,255,255,180);\">"
        + (item->channel.title.isEmpty() ? item->feed.name : item->channel.title) +
        "</span></p></body></html>"
    );
    //channel description
    ui->chDesc->setText(
        "<html><head/><body><p>"
        "<span style=\"font-size:32pt; font-weight:200; color:rgba(255,255,255,150);\">"
        + (item->channel.desc.isEmpty() ? item->feed.desc : item->channel.desc) +
        "</span></p></body></html>"
    );
    #else
    //channel title
    ui->chTitle->setText(
        "<html><head/><body><p>"
        "<span style=\"font-size:38pt; font-weight:200; color:rgba(255,255,255,180);\">"
        + (item->channel.title.isEmpty() ? item->feed.name : item->channel.title) +
        "</span></p></body></html>"
    );
    //channel description
    ui->chDesc->setText(
        "<html><head/><body><p>"
        "<span style=\"font-size:28pt; font-weight:200; color:rgba(255,255,255,150);\">"
        + (item->channel.desc.isEmpty() ? item->feed.desc : item->channel.desc) +
        "</span></p></body></html>"
    );
    #endif
}

/**
 * @brief Called when settings was changed
 * @param tag key of item in settings
 */
void NewsViewWidget::settingsChanged(QString tag){
    if(tag == "view_title_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setBold(true);
        font.setPointSize(sm.getInt(tag));
        ui->titleLabel->setFont(font);
    }
    else if(tag == "view_text_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setPointSize(sm.getInt(tag));
        ui->textLabel->setFont(font);
    }
    else if(tag == "view_footer_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setItalic(true);
        font.setPointSize(sm.getInt(tag));
        ui->timeLabel->setFont(font);
        ui->linkLabel->setFont(font);
        ui->feedNameLabel->setFont(font);
        ui->guidLabel->setFont(font);
    }
}
