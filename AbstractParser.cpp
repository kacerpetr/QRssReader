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

#include <QXmlStreamReader>
#include <QDebug>
#include "AbstractParser.h"

/**
 * @brief Parses version from given rss/xml string
 * @param xml rss feed content
 */
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
