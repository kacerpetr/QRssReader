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
#include <QMessageBox>
#include "StorageAccess.h"

/**
 * @brief Compares two news items
 */
bool operator==(const NewsItem& item1, const NewsItem& item2){
    if(item1.title == item2.title && item1.feed.name == item2.feed.name){
        return true;
    }
    return false;
}

/**
 * @brief Class constructor
 * @param parent
 */
RssDataModel::RssDataModel(QObject* parent) : RssFeedModel(parent),
networkReply(NULL), timeoutTimer(NULL){
    manager = new QNetworkAccessManager();
    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));
}

/**
 * @brief Class destructor
 */
RssDataModel::~RssDataModel(){
    delete manager;
    manager = NULL;
}

/**
 * @brief RssDataModel::setFolder
 * @param folder
 */
void RssDataModel::setCacheFolder(const QString& folder){
    this->cacheFolder = folder;
}

/**
 * @brief Starts downloading rss data
 * Adds feed urls to queue
 */
void RssDataModel::downloadRssData(){
    //list of feeds
    const QList<FeedItem>& feeds = feedList();

    //feedlist empty
    if(feeds.isEmpty()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Information");
        msgBox.setText("Feedlist for current tab is empty");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    //adds feed urls to queue
    for(int i = 0; i < feeds.length(); i++){
        if(!feeds[i].enabled) continue;
        loadingQueue.append(feeds[i].url);
    }

    //clears rss data
    news.clear();

    //start loading of first feed
    loadFeed(loadingQueue.takeFirst());
}

/**
 * @brief Loads rss on given url
 * @param url address of rss source
 */
void RssDataModel::loadFeed(QString url){
    networkReply = manager->get(QNetworkRequest(QUrl(url)));
    int progress = 100 - (loadingQueue.length()+1)*100 / feedList().length();
    timeoutTimer->start(REQUEST_TIMEOUT);
    emit loadingStarted(url, progress);
}

/**
 * @brief Stops feed download after defined interval runs out
 */
void RssDataModel::requestTimeout(){
    networkReply->abort();
    networkReply = NULL;
    timeoutTimer->stop();
}

/**
 * @brief Called when rss data download is completed
 * @param reply
 */
void RssDataModel::replyFinished(QNetworkReply* reply){
    //clears internal reply pointer (used for timeout check)
    networkReply = NULL;

    //stops timeout timer
    timeoutTimer->stop();

    //gets feed item by its url
    if(reply->error() == QNetworkReply::NoError && reply->isOpen()){
        const FeedItem* feed = feedByUrl(reply->url().toString());
        QString xml = reply->readAll();
        if(!xml.isEmpty()){
            parseRss(xml, *feed);
            saveRss(xml, *feed);
        }
    }

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

/**
 * @brief Adds item to list of news
 * @param item
 */
void RssDataModel::addNewsItem(const NewsItem& item){
    QMap<QDate, NewsItem>::iterator it = news.find(item.time.date(), item);

    //adds item if it is not in list
    if(it == news.end()){
        news.insert(item.time.date(), item);
    }
}

/**
 * @brief Parsing of dowloaded or loaded rss data
 * @param xml
 * @param feed
 */
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
                    item.title = "";
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
                        item.title = rd.text().toString();
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

/**
 * @brief Loads rss data from chache
 * Ususaly called at program startup
 */
void RssDataModel::loadRssCache(){
    Q_ASSERT_X(!this->cacheFolder.isEmpty(), "loadRss()", "No cache folder was set");

    //feed list
    const QList<FeedItem>& feeds = feedList();

    //loads data
    for(int i = 0; i < feeds.length(); i++){
        QString filename = feeds[i].name.toLower().replace(" ", "_");
        QString relativePath = QString(this->cacheFolder) + "/" + filename + ".xml";
        if(!StorageAccess::get().exists(relativePath)) continue;
        if(!feeds[i].enabled) continue;
        QString xml;
        StorageAccess::get().readString(xml, relativePath);
        parseRss(xml, feeds[i]);
    }

    //data changed signal
    emit dataChanged();
}

/**
 * @brief Saves xml data to cache
 * @param xml downloaded data
 * @param feed asociated feed
 */
void RssDataModel::saveRss(const QString& xml, const FeedItem& feed) const{
    Q_ASSERT_X(!this->cacheFolder.isEmpty(), "saveRss()", "No cache folder was set");

    //creates data folder if does not exists
    StorageAccess::get().mkDir(this->cacheFolder);

    //saves data
    QString filename = feed.name.toLower().replace(" ", "_");
    QString relativePath = QString(this->cacheFolder) + "/" + filename + ".xml";
    StorageAccess::get().writeString(xml, relativePath);
}

/**
 * Returns reference to downloaded rss data
 */
const QMultiMap<QDate,NewsItem>& RssDataModel::data() const{
    return news;
}

/**
 * Returns name of cache folder
 */
QString RssDataModel::rssCacheFolder() const{
    return cacheFolder;
}
