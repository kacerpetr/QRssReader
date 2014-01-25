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

bool operator==(const FeedItem& item1, const FeedItem& item2){
    if(item1.name == item2.name) return true;
    if(item1.url == item2.url) return true;
    return false;
}

RssFeedModel::RssFeedModel(QString filename) : filename(filename){}

const QList<FeedItem>& RssFeedModel::feedList() const{
    return feeds;
}

void RssFeedModel::saveFeedList() const{
    //opens xml writer
    QXmlStreamWriter* wr = StorageAccess::get().openXmlWriter(filename);
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
        wr->writeTextElement("description", feeds[i].description);
        wr->writeTextElement("enabled", feeds[i].enabled==true?"true":"false");

        //background color in news list
        QString bkgClr = QString::number(feeds[i].bkgColor.red()) + ",";
        bkgClr += QString::number(feeds[i].bkgColor.green()) + ",";
        bkgClr += QString::number(feeds[i].bkgColor.blue());
        wr->writeTextElement("bkgColor", bkgClr);

        //text color in news list
        QString textClr = QString::number(feeds[i].textColor.red()) + ",";
        textClr += QString::number(feeds[i].textColor.green()) + ",";
        textClr += QString::number(feeds[i].textColor.blue());
        wr->writeTextElement("textColor", textClr);

        //end element
        wr->writeEndElement();
    }

    //end of main element
    wr->writeEndElement();

    //closes xml writer
    StorageAccess::get().closeXmlWriter(&wr);
}

void RssFeedModel::loadFeedList(){
    //opens xml reader
    QXmlStreamReader* rd = StorageAccess::get().openXmlReader(filename);
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
                    item.description = "";
                    item.enabled = false;
                    item.bkgColor = QColor(220, 220, 220);
                    item.textColor = QColor(20, 20, 20);
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
                    item.description = rd->text().toString();
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
                //text color in news list
                else if(elemName == "textColor"){
                    QString text = rd->text().toString();
                    QStringList clr = text.split(",");
                    if(clr.length() != 3) break;
                    unsigned char r = clr[0].toInt();
                    unsigned char g = clr[1].toInt();
                    unsigned char b = clr[2].toInt();
                    item.textColor = QColor(r, g, b);
                }
                break;

            default:
                break;
        }
    }

    //closes xml reader
    StorageAccess::get().closeXmlReader(&rd);
}

bool RssFeedModel::modifyFeed(int index, const FeedItem& feed){
    int sres = feeds.indexOf(feed);
    if(sres >= 0 && sres != index) return false;
    feeds.replace(index, feed);
    return true;
}

bool RssFeedModel::addFeed(const FeedItem& feed){
    int index = feeds.indexOf(feed);
    if(index >= 0) return false;
    feeds.append(feed);
    return true;
}

void RssFeedModel::removeFeed(int index){
    feeds.removeAt(index);
}

const FeedItem* RssFeedModel::byUrl(QString url){
    for(int i = 0; i < feeds.length(); i++){
        if(feeds[i].url == url) return &feeds.at(i);
    }
    return NULL;
}

QString RssFeedModel::feedListFile() const{
    return filename;
}
