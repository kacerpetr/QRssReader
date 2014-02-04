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

#ifndef RSSFEEDMODEL_H
#define RSSFEEDMODEL_H

#include <QString>
#include <QColor>
#include <QList>

/** Feed item structure */
typedef struct{
    QString name;
    QString url;
    QString description;
    QColor bkgColor;
    QColor textColor;
    bool enabled;
}FeedItem;

bool operator==(const FeedItem& item1, const FeedItem& item2);

/**
 * @brief The RssFeedModel class
 * Holds and manages list of feeds
 */
class RssFeedModel : public QObject{
    Q_OBJECT

    public:
        RssFeedModel(QObject* parent = 0);
        void setFeedListFileName(QString feedListFile);
        const QList<FeedItem>& feedList() const;
        void saveFeedList() const;
        void loadFeedList();
        bool modifyFeed(int index, const FeedItem& feed);
        bool addFeed(const FeedItem& feed);
        void removeFeed(int index);
        const FeedItem* feedByUrl(QString url);
        QString feedListFileName() const;

    private:
        QString feedListFile;
        QList<FeedItem> feeds;
};

#endif //RSSFEEDMODEL_H
