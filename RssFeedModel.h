#ifndef RSSFEEDMODEL_H
#define RSSFEEDMODEL_H

#include <QString>
#include <QColor>
#include <QList>

typedef struct{
    QString name;
    QString url;
    QString description;
    QColor bkgColor;
    QColor textColor;
    bool enabled;
}FeedItem;

bool operator==(const FeedItem& item1, const FeedItem& item2);

class RssFeedModel{
    public:
        RssFeedModel(QString filename);
        const QList<FeedItem>& feedList() const;
        void saveFeedList() const;
        void loadFeedList();
        bool modifyFeed(int index, const FeedItem& feed);
        bool addFeed(const FeedItem& feed);
        void removeFeed(int index);
        const FeedItem* byUrl(QString url);

    private:
        QString filename;
        QList<FeedItem> feeds;
};

#endif // RSSFEEDMODEL_H
