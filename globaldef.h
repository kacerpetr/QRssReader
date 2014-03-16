#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include <QString>
#include <QList>
#include <QDateTime>
#include <QColor>

#define SETTINGS_FILE "settings.xml"       /**< Name of application settings file */
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
