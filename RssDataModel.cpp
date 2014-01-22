#include "RssDataModel.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <QTextStream>

RssDataModel::RssDataModel(QObject* parent) : QObject(parent){
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

RssDataModel::~RssDataModel(){
    delete manager;
    manager = NULL;
}

void RssDataModel::setFeedModel(RssFeedModel* feedModel){
    this->feedModel = feedModel;
}

void RssDataModel::loadRssData(){
    const QList<FeedItem>& feeds = feedModel->feedList();
    for(int i = 0; i < feeds.length(); i++){
        manager->get(QNetworkRequest(QUrl(feeds[i].url)));
    }
}

void RssDataModel::replyFinished(QNetworkReply* reply){
    const FeedItem* feed = feedModel->byUrl(reply->url().toString());
    parseRss(reply->readAll(), *feed);
    emit dataChanged();
}

void RssDataModel::addNewsItem(const NewsItem& item){
    news.insert(item.time.date(), item);
}

void RssDataModel::parseRss(QString xml, const FeedItem& feed){
    QXmlStreamReader rd(xml);

    //news item
    NewsItem item;
    item.feed = feed;

    //support variables
    QString elemName;
    int state = 0;

    //parsing file
    while(!rd.atEnd()){
        switch(rd.readNext()){
            case QXmlStreamReader::StartElement:
                elemName = rd.name().toString();
                if(rd.name() == "item"){
                    state = 1;
                    item.header = "";
                    item.text = "";
                    item.time = QDateTime();
                }
                break;

            case QXmlStreamReader::EndElement:
                elemName = "";
                if(rd.name() == "item"){
                    addNewsItem(item);
                    state = 0;
                }
                break;

            case QXmlStreamReader::Characters:
                if(elemName.isEmpty()) break;

                //rss item parsing
                if(state == 1){
                    //item title
                    if(elemName == "title"){
                        item.header = rd.text().toString();
                    }
                    //item description
                    else if(elemName == "description"){
                        item.text = rd.text().toString();
                    }
                    //item datetime
                    else if(elemName == "pubDate"){
                        QString str = rd.text().toString();
                        str.chop(4);
                        QString format = "ddd, dd MMM yyyy hh:mm:ss";
                        QLocale loc(QLocale::English);
                        QDateTime dt = loc.toDateTime(str, format);
                        item.time = dt;
                    }
                }

            default:
                break;
        }
    }
}

const QMultiMap<QDate,NewsItem>& RssDataModel::data() const{
    return news;
}

