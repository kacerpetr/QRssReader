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

#ifndef RSSDATAMODEL_H
#define RSSDATAMODEL_H

#include <QString>
#include <QDateTime>
#include <QMultiMap>
#include <QColor>
#include <QNetworkReply>
#include "RssFeedModel.h"

typedef struct{
    QString title;
    QString text;
    QDateTime time;
    FeedItem feed;
    QString link;
    QString guid;
}NewsItem;

bool operator==(const NewsItem& item1, const NewsItem& item2);

class RssDataModel : public QObject{
    Q_OBJECT

    public:
        RssDataModel(QObject* parent = 0);
        ~RssDataModel();
        void setFeedModel(RssFeedModel* feedModel);
        void setFolder(const QString& folder);
        void downloadRssData();
        const QMultiMap<QDate,NewsItem>& data() const;
        void loadRss();
        QString dataFolder() const;

    public slots:
        void replyFinished(QNetworkReply* reply);

    signals:
        void dataChanged();
        void loadingStarted(QString feed, int progress);
        void loadingFinished();

    private:
        void addNewsItem(const NewsItem& item);
        void parseRss(const QString& xml, const FeedItem& feed);
        void saveRss(const QString& xml, const FeedItem& feed) const;
        void loadFeed(QString url);
        void newsToXml();

    private:
        QMultiMap<QDate,NewsItem> news;
        QNetworkAccessManager* manager;
        RssFeedModel* feedModel;
        QList<QString> loadingQueue;
        QString folder;
};

#endif // RSSDATAMODEL_H
