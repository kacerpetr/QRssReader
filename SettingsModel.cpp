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

#include "SettingsModel.h"
#include "StorageAccess.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

/**
 * @brief Returns reference of model object
 */
SettingsModel& SettingsModel::get(){
    static SettingsModel instance;
    return instance;
}

/**
 * @brief Loads setting file
 * If file does not exist, copy of default settings file is created
 */
void SettingsModel::loadSettings(){
    //settings file does not exist, default configuration will be used
    if(!StorageAccess::get().exists(SETTINGS_FILE)){
        //error handling vars
        bool success = false;
        QString errStr = "";

        //makes copy of default settings file for by platform
        #ifdef ANDROID
            QFile source(":/settings/settings_android");
            success = source.copy(StorageAccess::get().absPath(SETTINGS_FILE));
            errStr = source.errorString();
        #else
            QFile source(":/settings/settings_pc");
            success = source.copy(StorageAccess::get().absPath(SETTINGS_FILE));
            errStr = source.errorString();
        #endif

        //error check
        if(!success){
            QMessageBox msgBox;
            msgBox.setWindowTitle("Load settings error");
            msgBox.setText("Error creating copy of default settings");
            msgBox.setInformativeText(errStr);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }

        //changes permission - eneble writing for owner
        #ifndef ANDROID
            QFile file(StorageAccess::get().absPath(SETTINGS_FILE));
            if(!file.setPermissions(QFileDevice::WriteOwner |
            QFileDevice::ReadOwner | QFileDevice::ReadGroup | QFileDevice::ReadOther)){
                QMessageBox msgBox;
                msgBox.setWindowTitle("Load settings error");
                msgBox.setText("Unable to set settings file permissions");
                msgBox.setInformativeText(file.errorString());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }
        #endif
    }

    //opens xml reader
    QXmlStreamReader* rd = StorageAccess::get().openXmlReader(SETTINGS_FILE);
    if(rd == NULL) return;

    //support variables
    QString elemName;

    //parsing file
    while(!rd->atEnd()){
        switch(rd->readNext()){
            case QXmlStreamReader::StartElement:
                elemName = rd->name().toString();
                break;

            case QXmlStreamReader::EndElement:
                elemName = "";
                break;

            case QXmlStreamReader::Characters:
                if(elemName.isEmpty()) break;
                if(elemName == "settings") break;
                data.insert(elemName, rd->text().toString());

            default:
                break;
        }
    }

    //closes xml reader
    StorageAccess::get().closeXmlReader(&rd);
}

/**
 * @brief Saves settings to xml file
 */
void SettingsModel::saveSettings(){
    //opens xml writer
    QXmlStreamWriter* wr = StorageAccess::get().openXmlWriter(SETTINGS_FILE);
    if(wr == NULL) return;

    //beginning of xml writing
    wr->setAutoFormatting(true);
    wr->writeStartDocument();
    wr->writeStartElement("settings");

    //writes data to xml file
    QMap<QString,QString>::iterator it = data.begin();
    while(it != data.end()){
        wr->writeTextElement(it.key(), it.value());
        it++;
    }

    //end of main element
    wr->writeEndElement();

    //closes xml writer
    StorageAccess::get().closeXmlWriter(&wr);
}

/**
 * @brief Settings item value setter
 * @param tag key of item
 * @param value value of item
 */
void SettingsModel::setValue(const QString& tag, const QString& value){
    data.insert(tag, value);
    emit dataChanged(tag);
}

/**
 * @brief Settings item value setter
 * @param tag key of item
 * @param value value of item
 */
void SettingsModel::setValue(const QString& tag, const int value){
    data.insert(tag, QString::number(value));
    emit dataChanged(tag);
}

/**
 * @brief Settings item value setter
 * @param tag key of item
 * @param value value of item
 */
void SettingsModel::setValue(const QString& tag, const double value){
    data.insert(tag, QString::number(value));
    emit dataChanged(tag);
}

/**
 * @brief Settings value getter
 * @param tag key of settings item
 */
QString SettingsModel::getText(const QString& tag) const{
    return data[tag];
}

/**
 * @brief Settings value getter
 * @param tag key of settings item
 */
int SettingsModel::getInt(const QString& tag) const{
    return data[tag].toInt();
}

/**
 * @brief Settings value getter
 * @param tag key of settings item
 */
double SettingsModel::getDouble(const QString& tag) const{
    return data[tag].toDouble();
}
