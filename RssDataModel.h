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
#include <QTimer>
#include "RssFeedModel.h"
#include "globaldef.h"

bool operator==(const TRssItem& item1, const TRssItem& item2);

/**
 * @brief The RssDataModel class
 * Stores, saves, loads and downloads rss data
 */
class RssDataModel : public RssFeedModel{
    Q_OBJECT

    public:
        RssDataModel(QObject* parent = 0);
        ~RssDataModel();
        void setCacheFolder(const QString& cacheFolder);
        void downloadRssData();
        const QMultiMap<QDate,TRssItem>& data() const;
        void loadRssCache(bool noEmit = false);
        QString rssCacheFolder() const;
        void loadRssFile(QString relativePath, int feedIndex);

    public slots:
        void replyFinished(QNetworkReply* reply);
        void requestTimeout();

    signals:
        void dataChanged();
        void loadingStarted(QString feed, int progress);
        void loadingFinished();

    private:
        void addNewsItem(const TRssItem& item);
        void parseRss(const QString& xml, const FeedItem& feed);
        void saveRss(const QString& xml, const FeedItem& feed) const;
        void loadFeed(QString url);
        void newsToXml();

    private:
        QMultiMap<QDate,TRssItem> news;
        QNetworkAccessManager* manager;
        QList<QString> loadingQueue;
        QString cacheFolder;
        QNetworkReply* networkReply;
        QTimer* timeoutTimer;
        bool clearDataFlag;
};

#endif // RSSDATAMODEL_H
