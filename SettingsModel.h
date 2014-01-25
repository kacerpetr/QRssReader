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

#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QObject>
#include <QString>
#include <QMap>

#define SETTINGS_FILE "settings.xml"

class SettingsModel : public QObject{
    Q_OBJECT

    public:
        static SettingsModel& get();
        void loadSettings();
        void saveSettings();
        void setValue(const QString& tag, const QString& value);
        void setValue(const QString& tag, const int value);
        void setValue(const QString& tag, const double value);
        QString getText(const QString& tag) const;
        int getInt(const QString& tag) const;
        double getDouble(const QString& tag) const;

     signals:
        void dataChanged(QString);

     private:
        QMap<QString,QString> data;

     private:
        SettingsModel(QObject* parent = 0):QObject(parent){}
        SettingsModel(SettingsModel const&);
        void operator=(SettingsModel const&);
};

#endif // SETTINGSMODEL_H
