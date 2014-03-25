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

#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include <QString>
#include <QList>
#include <QDateTime>
#include <QColor>

#define APP_FOLDER (".qrssreader")
#define SETTINGS_FILE "settings.xml"
#define FEEDS_PREFIX (QString(":/feedlist/feedlist"))
#define RSS_FEED_FILE "feedlist"
#define RSS_DATA_FOLDER "RssData"
#define MAX_TAB_COUNT 7
#define REQUEST_TIMEOUT 4000
#define DEFAULT_LIST_NAME (QString("/default"))

/** Feed item structure */
typedef struct{
    QString name;
    QString url;
    QString desc;
    QColor bkgColor;
    bool enabled;
}FeedItem;

/** Feed list description */
typedef struct{
    QString name;
    QString desc;
    QString url;
}FeedList;

/**
 * @brief The ERssVersion enum
 */
enum ERssVersion{
    Rss10 = 10,
    Rss20 = 20
};

/**
 * @brief Image of rss chanel
 */
typedef struct{
    QString url;
    QString title;
    QString link;
}TRssImage;

/**
 * @brief Informations about rss chanel
 */
typedef struct{
    QString title;
    QString link;
    QString desc;
    TRssImage image;
    ERssVersion version;
}TRssChanel;

/**
 * @brief One item from all in chanel
 */
typedef struct{
    QString title;
    QString link;
    QString desc;
    QDateTime dt;
    QString guid;
    TRssChanel channel;
    FeedItem feed;
}TRssItem;

#endif // GLOBALDEF_H
