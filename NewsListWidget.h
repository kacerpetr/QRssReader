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

#ifndef NEWSLISTWIDGET_H
#define NEWSLISTWIDGET_H

#include <QScrollArea>
#include <QVBoxLayout>

#include "NewsItemWidget.h"
#include "NewsGroupWidget.h"
#include "RssDataModel.h"

/**
 * @brief The NewsListWidget class
 * Shows list of downloaded news
 */
class NewsListWidget : public QScrollArea{
    Q_OBJECT

    public:
        explicit NewsListWidget(QWidget* parent = 0);
        void createList(const QMultiMap<QDate,NewsItem>& news);
        void clearList();
        NewsItem* selectFirst();
        NewsItem* selectNext();
        NewsItem* selectPrev();
        NewsItem* selectLast();

    signals:
        void pressed(NewsItem* item);

    private slots:
        void itemPressed(NewsItemWidget* item);

    private:
        QWidget* content;
        QVBoxLayout* layout;
        QList<QWidget*> allItems;
        QList<NewsItemWidget*> newsItems;
};

#endif // NEWSLISTWIDGET_H
