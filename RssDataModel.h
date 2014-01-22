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
}NewsItem;

class RssDataModel : public QObject{
    Q_OBJECT

    public:
        RssDataModel(QObject* parent = 0);
        ~RssDataModel();
        void setFeedModel(RssFeedModel* feedModel);
        void loadRssData();
        void addNewsItem(const NewsItem& item);
        void parseRss(QString xml, const FeedItem& feed);
        const QMultiMap<QDate,NewsItem>& data() const;

    public slots:
        void replyFinished(QNetworkReply* reply);

    signals:
        void dataChanged();

    private:
        QMultiMap<QDate,NewsItem> news;
        QNetworkAccessManager* manager;
        RssFeedModel* feedModel;
};

#endif // RSSDATAMODEL_H
