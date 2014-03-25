#include "Rss10Parser.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <SettingsModel.h>

Rss10Parser::Rss10Parser(){}

void Rss10Parser::parseRss(const QString& xml, QList<TRssItem>& data){
    QXmlStreamReader rd(xml);

    //support variables
    QString elemName;
    TRssItem item;
    TRssChanel channel;
    int state = 0;

    //default channel values
    channel.desc = QString();
    channel.link = QString();
    channel.title = QString();
    channel.version = Rss20;
    channel.image.link = QString();
    channel.image.title = QString();
    channel.image.url = QString();

    //parsing file
    while(!rd.atEnd()){
        switch(rd.readNext()){
            case QXmlStreamReader::StartElement:
                elemName = rd.name().toString();
                if(state != 0) break;
                if(elemName == "channel") state = 1; else
                if(elemName == "image") state = 2; else
                if(elemName == "item"){
                    //empty item
                    item.desc = QString();
                    item.dt = QDateTime();
                    item.guid = QString();
                    item.link = QString();
                    item.channel = channel;
                    item.title = QString();
                    //state for parsing item
                    state = 3;
                }
                break;

            case QXmlStreamReader::EndElement:
                elemName = "";
                if(rd.name() == "channel") state = 0; else
                if(rd.name() == "image") state = 0; else
                if(rd.name() == "item"){
                    data.append(item);
                    state = 0;
                }
                break;

            case QXmlStreamReader::Characters:
                if(elemName.isEmpty()) break;
                //channel description
                if(state == 1){
                    if(elemName == "description") channel.desc = rd.text().toString().trimmed(); else
                    if(elemName == "link") channel.link = rd.text().toString(); else
                    if(elemName == "title") channel.title = rd.text().toString();
                }
                //channel image
                if(state == 2){
                    if(elemName == "title") channel.image.title = rd.text().toString(); else
                    if(elemName == "link") channel.image.link = rd.text().toString(); else
                    if(elemName == "url") channel.image.url = rd.text().toString();
                }
                //item parsing
                if(state == 3){
                    if(elemName == "title") item.title = rd.text().toString(); else
                    if(elemName == "link") item.link = rd.text().toString(); else
                    if(elemName == "description") item.desc = rd.text().toString(); else
                    if(elemName == "date") item.dt = parseDateTime(rd.text().toString());
                }

            default:
                break;
        }
    }
}

QDateTime Rss10Parser::parseDateTime(const QString& str) const{
    return QDateTime::fromString(str, "yyyy-MM-ddThh:mm:ssZ");
}
