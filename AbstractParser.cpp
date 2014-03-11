#include "AbstractParser.h"
#include <QXmlStreamReader>
#include <QDebug>

ERssVersion AbstractParser::parseVersion(const QString& xml){
    QXmlStreamReader rd(xml);

    //support variables
    QString elemName;

    //parsing file
    while(!rd.atEnd()){
        switch(rd.readNext()){
            case QXmlStreamReader::StartElement:
                elemName = rd.name().toString();
                if(elemName.toLower() == "rss") return Rss20; else
                if(elemName.toLower() == "rdf") return Rss10; else
                if(elemName.toLower() == "item") break;
                break;

            default:
                break;
        }
    }

    //unknown rss version
    return Rss20;
}
