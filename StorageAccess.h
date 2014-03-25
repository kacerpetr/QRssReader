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

#ifndef STORAGEACCESS_H
#define STORAGEACCESS_H

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringList>
#include <globaldef.h>

/**
 * @brief The StorageAccess class
 * Support functions for working with files, singleton
 */
class StorageAccess{
    public:
        static StorageAccess& get();
        QString appDir() const;
        QString absPath(QString filename) const;
        QXmlStreamReader* openXmlReader(QString filename) const;
        void closeXmlReader(QXmlStreamReader** reader) const;
        QXmlStreamWriter* openXmlWriter(QString filename) const;
        void closeXmlWriter(QXmlStreamWriter** writer) const;
        bool writeString(const QString& str, const QString& relativePath) const;
        bool readString(QString& result, const QString& relativePath) const;
        bool mkDir(const QString& relativePath) const;
        bool exists(const QString& relativePath) const;
        bool clearDir(const QString& relativePath) const;
        bool rmFile(const QString& relativePath) const;

     private:
        StorageAccess(){}
        StorageAccess(StorageAccess const&);
        void operator=(StorageAccess const&);
};

#endif // STORAGEACCESS_H
