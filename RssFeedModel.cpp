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

#include "RssFeedModel.h"
#include "StorageAccess.h"
#include <QMessageBox>
#include <QFile>
#include <QDirIterator>

/**
 * @brief Compares two feed items
 */
bool operator==(const FeedItem& item1, const FeedItem& item2){
    if(item1.name == item2.name) return true;
    if(item1.url == item2.url) return true;
    return false;
}

/**
 * @brief Class constructor
 * @param filename
 */
RssFeedModel::RssFeedModel(QObject* parent) : QObject(parent), defaultList(0){}

/**
 * @brief Sets name of feedlist file
 * @param filename
 */
void RssFeedModel::setFeedListFileName(QString filename){
    this->feedListFile = filename;
}

/**
 * @brief Returns reference to list of feeds
 */
const QList<FeedItem>& RssFeedModel::feedList() const{
    return feeds;
}

/**
 * @brief Saves feedlist to xml
 */
void RssFeedModel::saveFeedList() const{
    Q_ASSERT_X(!this->feedListFile.isEmpty(), "saveFeedList()", "No feedlist file was set");

    //opens xml writer
    QXmlStreamWriter* wr = StorageAccess::get().openXmlWriter(feedListFile);
    if(wr == NULL) return;

    //beginning of xml writing
    wr->setAutoFormatting(true);
    wr->writeStartDocument();
    wr->writeStartElement("feedlist");

    //writes feed list to xml file
    for(int i = 0; i < feeds.length(); i++){
        //start element
        wr->writeStartElement("feed");

        //main properties
        wr->writeTextElement("name", feeds[i].name);
        wr->writeTextElement("url", feeds[i].url);
        wr->writeTextElement("description", feeds[i].desc);
        wr->writeTextElement("enabled", feeds[i].enabled==true?"true":"false");

        //background color in news list
        QString bkgClr = QString::number(feeds[i].bkgColor.red()) + ",";
        bkgClr += QString::number(feeds[i].bkgColor.green()) + ",";
        bkgClr += QString::number(feeds[i].bkgColor.blue());
        wr->writeTextElement("bkgColor", bkgClr);

        //end element
        wr->writeEndElement();
    }

    //end of main element
    wr->writeEndElement();

    //closes xml writer
    StorageAccess::get().closeXmlWriter(&wr);
}

/**
 * @brief Default list index setter
 * @param defaultList index of predefined feedlist
 */
void RssFeedModel::setDefaultList(int defaultList){
    this->defaultList = defaultList;
}

/**
 * @brief Creates copy of default feedlist
 * @return true if success, false otherwise
 */
bool RssFeedModel::copyDefaultList(){
    //no default list set
    if(defaultList == 0) return false;

    //storage access singleton reference
    StorageAccess& sa = StorageAccess::get();

    //predefined feedlist path
    QString dflPath = FEEDS_PREFIX + DEFAULT_LIST_NAME + QString::number(defaultList) + ".xml";

    //error handling vars
    bool success = false;

    //creates copy of predefined feedlist to current feedlist
    QFile source(dflPath);
    success = source.copy(sa.absPath(feedListFile));

    //error check
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Load default error");
        msgBox.setText("Error creating copy of default feed list file");
        msgBox.setInformativeText(source.errorString());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    //sets file permissions
    #ifndef ANDROID
        QFile file(sa.absPath(feedListFile));
        success = file.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner |
                                      QFileDevice::ReadGroup | QFileDevice::ReadOther);

        //error check
        if(!success){
            QMessageBox msgBox;
            msgBox.setWindowTitle("Load default error");
            msgBox.setText("Unable to set feedlist file permissions");
            msgBox.setInformativeText(file.errorString());
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    #endif

    //success
    return true;
}

/**
 * @brief Loads feedlist from xml
 * @param defaultList index of predefined feedlist
 */
void RssFeedModel::loadFeedList(){
    Q_ASSERT_X(!this->feedListFile.isEmpty(), "loadFeedList()", "No feedlist file was set");

    //clears feedlist
    feeds.clear();

    //loads default feedlist if file does not exists
    if(!StorageAccess::get().exists(feedListFile)){
        if(defaultList == 0) return;
        if(!copyDefaultList()) return;
    }

    //opens xml reader
    QXmlStreamReader* rd = StorageAccess::get().openXmlReader(feedListFile);
    if(rd == NULL) return;

    //support variables
    FeedItem item;
    QString elemName;
    int state = 0;

    //parsing file
    while(!rd->atEnd()){
        switch(rd->readNext()){
            case QXmlStreamReader::StartElement:
                elemName = rd->name().toString();
                if(rd->name() == "feed"){
                    //state - parsing feed item
                    state = 1;
                    //default feed properties
                    item.name = "";
                    item.url = "";
                    item.desc = "";
                    item.enabled = false;
                    item.bkgColor = QColor(220, 220, 220);
                }
                break;

            case QXmlStreamReader::EndElement:
                elemName = "";
                if(rd->name() == "feed"){
                    state = 0;
                    addFeed(item);
                }
                break;

            case QXmlStreamReader::Characters:
                if(elemName.isEmpty()) break;
                if(state != 1) break;

                //feed name
                if(elemName == "name"){
                    item.name = rd->text().toString();
                }
                //feed url
                else if(elemName == "url"){
                    item.url = rd->text().toString();
                }
                //feed description
                else if(elemName == "description"){
                    item.desc = rd->text().toString();
                }
                //feed enabled/disabled
                else if(elemName == "enabled"){
                    if(rd->text().toString() == "true")
                        item.enabled = true;
                    else
                        item.enabled = false;
                }
                //background color in news list
                else if(elemName == "bkgColor"){
                    QString text = rd->text().toString();
                    QStringList clr = text.split(",");
                    if(clr.length() != 3) break;
                    unsigned char r = clr[0].toInt();
                    unsigned char g = clr[1].toInt();
                    unsigned char b = clr[2].toInt();
                    item.bkgColor = QColor(r, g, b);
                }
                break;

            default:
                break;
        }
    }

    //closes xml reader
    StorageAccess::get().closeXmlReader(&rd);
}

/**
 * @brief RssFeedModel::modifyFeed
 * @param index index of feed to modify
 * @param feed new values
 * @return true if success, false otherwise
 */
bool RssFeedModel::modifyFeed(int index, const FeedItem& feed){
    int sres = feeds.indexOf(feed);
    if(sres >= 0 && sres != index) return false;
    feeds.replace(index, feed);
    return true;
}

/**
 * @brief Adds new feed to list
 * @param feed
 */
bool RssFeedModel::addFeed(const FeedItem& feed){
    int index = feeds.indexOf(feed);
    if(index >= 0) return false;
    feeds.append(feed);
    return true;
}

/**
 * @brief Removes feed from list
 * @param index
 */
void RssFeedModel::removeFeed(int index){
    feeds.removeAt(index);
}

/**
 * @brief Returns pointer to feed with given url
 * @param url
 */
const FeedItem* RssFeedModel::feedByUrl(QString url){
    for(int i = 0; i < feeds.length(); i++){
        if(feeds[i].url == url) return &feeds.at(i);
    }
    return NULL;
}

/**
 * @brief Returns name of feedlist xml file
 */
QString RssFeedModel::feedListFileName() const{
    return feedListFile;
}

/**
 * @brief Fills given array with info about predefined feedlists
 * @param list list to be filled
 */
bool RssFeedModel::predefinedFeedlist(QList<FeedList>& lists){
    //creates list of predefined feedlist containing only urls
    QDirIterator it(FEEDS_PREFIX, QDirIterator::Subdirectories);
    while(it.hasNext()){
        FeedList list;
        list.url = it.next();
        lists.append(list);
    }

    //loads feedlists names and descriptions
    for(int i = 0; i < lists.length(); i++){
        //opens xml reader
        QXmlStreamReader* rd = StorageAccess::get().openXmlReader(lists[i].url);
        if(rd == NULL) return false;

        //support variables
        QString elemName;
        int state = 0;

        //parsing file
        while(!rd->atEnd()){
            if(state == 2) break;
            switch(rd->readNext()){
                case QXmlStreamReader::StartElement:
                    elemName = rd->name().toString();
                    if(rd->name() == "header"){
                        state = 1;
                    }
                    break;

                case QXmlStreamReader::EndElement:
                    elemName = "";
                    if(rd->name() == "header"){
                        state = 2;
                    }
                    break;

                case QXmlStreamReader::Characters:
                    if(elemName.isEmpty()) break;
                    if(state != 1) break;
                    //feedlist name
                    if(elemName == "name"){
                        lists[i].name = rd->text().toString();
                    }
                    //feedlist description
                    else if(elemName == "description"){
                        lists[i].desc = rd->text().toString();
                    }
                    break;

                default:
                    break;
            }
        }

        //closes xml reader
        StorageAccess::get().closeXmlReader(&rd);
    }

    return true;
}
