#include "Rss20Parser.h"
#include <QDebug>
#include <QXmlStreamReader>

Rss20Parser::Rss20Parser(){}

void Rss20Parser::parseRss(const QString& xml, QList<TRssItem>& data){
    QXmlStreamReader rd(xml);

    //support variables
    QString elemName;
    TRssChanel channel;
    TRssItem item;
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
                //image parsing
                if(state == 0 && elemName == "image"){
                    state = 1;
                }
                //item parsing
                else if(state == 0 && elemName == "item"){
                    //empty item
                    item.desc = QString();
                    item.dt = QDateTime();
                    item.guid = QString();
                    item.link = QString();
                    item.channel = channel;
                    item.title = QString();
                    //state for item parsing
                    state = 2;
                }
                break;

            case QXmlStreamReader::EndElement:
                elemName = "";
                //image parsing
                if(rd.name() == "image"){
                    state = 0;
                }
                //item parsing
                else if(rd.name() == "item"){
                    data.append(item);
                    state = 0;
                }
                break;

            case QXmlStreamReader::Characters:
                if(elemName.isEmpty()) break;
                //channel description
                if(state == 0){
                    if(elemName == "description") channel.desc = rd.text().toString().trimmed(); else
                    if(elemName == "link") channel.link = rd.text().toString(); else
                    if(elemName == "title") channel.title = rd.text().toString();
                }
                //channel image
                if(state == 1){
                    if(elemName == "title") channel.image.title = rd.text().toString(); else
                    if(elemName == "link") channel.image.link = rd.text().toString(); else
                    if(elemName == "url") channel.image.url = rd.text().toString();
                }
                //item parsing
                if(state == 2){
                    if(elemName == "title") item.title = rd.text().toString(); else
                    if(elemName == "description") item.desc = rd.text().toString(); else
                    if(elemName == "link") item.link = rd.text().toString(); else
                    if(elemName == "guid") item.guid = rd.text().toString(); else
                    if(elemName == "pubDate") item.dt = parseDateTime(rd.text().toString());
                }

            default:
                break;
        }
    }
}

QDateTime Rss20Parser::parseDateTime(const QString& str) const{
    QString string(str);
    QString format = "ddd, dd MMM yyyy hh:mm:ss";
    if(string.length() > format.length())
        string.chop(string.length() - format.length());
    QLocale loc(QLocale::English);
    return loc.toDateTime(string, format);
}
