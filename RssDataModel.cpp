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

#include "RssDataModel.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QDir>
#include "StorageAccess.h"

bool operator==(const NewsItem& item1, const NewsItem& item2){
    if(item1.header == item2.header && item1.feed.name == item2.feed.name){
        return true;
    }
    return false;
}

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

void RssDataModel::setFolder(const QString& folder){
    this->folder = folder;
}

void RssDataModel::downloadRssData(){
    const QList<FeedItem>& feeds = feedModel->feedList();

    //adds feed urls to queue
    for(int i = 0; i < feeds.length(); i++){
        if(!feeds[i].enabled) continue;
        loadingQueue.append(feeds[i].url);
    }

    //start loading of first feed
    loadFeed(loadingQueue.takeFirst());
}

void RssDataModel::loadFeed(QString url){
    manager->get(QNetworkRequest(QUrl(url)));
    int progress = 100 - (loadingQueue.length()+1)*100 / feedModel->feedList().length();
    emit loadingStarted(url, progress);
}

void RssDataModel::replyFinished(QNetworkReply* reply){
    //gets feed item by its url
    const FeedItem* feed = feedModel->byUrl(reply->url().toString());
    QString xml = reply->readAll();
    parseRss(xml, *feed);
    saveRss(xml, *feed);

    //all feeds were loaded
    if(loadingQueue.length() == 0){
        emit dataChanged();
        emit loadingFinished();
    }
    //else load next feed
    else{
        loadFeed(loadingQueue.takeFirst());
    }
}

void RssDataModel::addNewsItem(const NewsItem& item){
    QMap<QDate, NewsItem>::iterator it = news.find(item.time.date(), item);

    //adds item if it is not in list
    if(it == news.end()){
        news.insert(item.time.date(), item);
    }
}

void RssDataModel::parseRss(const QString& xml, const FeedItem& feed){
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
                        QString format = "ddd, dd MMM yyyy hh:mm:ss";
                        if(str.length() > format.length())
                            str.chop(str.length() - format.length());
                        QLocale loc(QLocale::English);
                        QDateTime dt = loc.toDateTime(str, format);
                        item.time = dt;
                    }
                    //link to full article
                    else if(elemName == "link"){
                        item.link = rd.text().toString();
                    }
                    //guid of article
                    else if(elemName == "guid"){
                        item.guid = rd.text().toString();
                    }
                }

            default:
                break;
        }
    }
}

void RssDataModel::loadRss(){
    //feed list
    const QList<FeedItem>& feeds = feedModel->feedList();

    //loads data
    for(int i = 0; i < feeds.length(); i++){
        QString filename = feeds[i].name.toLower().replace(" ", "_");
        QString relativePath = QString(this->folder) + "/" + filename + ".xml";
        if(!StorageAccess::get().exists(relativePath)) continue;
        if(!feeds[i].enabled) continue;
        QString xml;
        StorageAccess::get().readString(xml, relativePath);
        parseRss(xml, feeds[i]);
    }

    //data changed signal
    emit dataChanged();
}

void RssDataModel::saveRss(const QString& xml, const FeedItem& feed) const{
    //creates data folder if does not exists
    StorageAccess::get().mkDir(this->folder);

    //saves data
    QString filename = feed.name.toLower().replace(" ", "_");
    QString relativePath = QString(this->folder) + "/" + filename + ".xml";
    StorageAccess::get().writeString(xml, relativePath);
}

const QMultiMap<QDate,NewsItem>& RssDataModel::data() const{
    return news;
}

QString RssDataModel::dataFolder() const{
    return folder;
}
