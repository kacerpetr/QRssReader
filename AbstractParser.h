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

#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include "globaldef.h"

/**
 * @brief The abstract rss parser class
 */
class AbstractParser{
    public:
        virtual void parseRss(const QString& xml, QList<TRssItem>& items) = 0;
        static ERssVersion parseVersion(const QString& xml);
};

#endif // ABSTRACTPARSER_H
