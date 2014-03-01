#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include <QString>
#include <QList>
#include <QDateTime>

#define SETTINGS_FILE "settings.xml"       /**< Name of application settings file */
#define FEEDS_PREFIX (QString(":/feedlist/feedlist"))
#define RSS_FEED_FILE "feedlist"
#define RSS_DATA_FOLDER "RssData"
#define TAB_COUNT 7
#define REQUEST_TIMEOUT 4000
#define DEFAULT_LIST_NAME (QString("/default"))

/**
 * @brief Image of rss chanel
 */
typedef struct{
    QString url;
    QString title;
    QString link;
    QString desc;
}TRssImage;

/**
 * @brief Informations about rss chanel
 */
typedef struct{
    QString title;
    QString link;
    QString desc;
    TRssImage image;
}TRssChanel;

/**
 * @brief One item from all in chanel
 */
typedef struct{
    QString title;
    QString link;
    QString desc;
    QDateTime dt;
    TRssChanel* src;
    QString guid;
}TRssItem;

/**
 * @brief Rss data structure
 */
typedef struct{
    TRssChanel chanel;
    QList<TRssItem> item;
}TRssData;

#endif // GLOBALDEF_H
