#ifndef RSSDATAMODEL_H
#define RSSDATAMODEL_H

#include <QString>
#include <QDateTime>
#include <QMultiMap>
#include <QColor>
#include <QNetworkReply>
#include "RssFeedModel.h"

typedef struct{
    QString header;
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
