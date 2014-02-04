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

#ifndef NEWSITEM_H
#define NEWSITEM_H

#include <QWidget>
#include <QColor>
#include "RssDataModel.h"
#include "NewsGroupWidget.h"

namespace Ui {
    class NewsItemWidget;
}

/**
 * @brief The NewsItemWidget class
 * Represents one report in list of news
 */
class NewsItemWidget : public QWidget{
    Q_OBJECT

    public:
        explicit NewsItemWidget(QWidget* parent = 0);
        ~NewsItemWidget();
        void setNewsItem(NewsItem item);
        void setSelected(bool selected);
        void setGroup(NewsGroupWidget* group);
        NewsGroupWidget* group() const;
        NewsItem* newsItem();
        bool isSelected() const;

    public slots:
        void settingsChanged(QString key);

    signals:
        void pressed(NewsItemWidget* item);

    private:
        void setColor(QColor bkgColor, QColor textColor);
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

    private:
        Ui::NewsItemWidget* ui;
        NewsItem item;
        bool selected;
        NewsGroupWidget* groupWidget;
};

#endif // NEWSITEM_H
