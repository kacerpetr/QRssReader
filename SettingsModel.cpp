#include "SettingsModel.h"
#include "StorageAccess.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

SettingsModel& SettingsModel::get(){
    static SettingsModel instance;
    return instance;
}

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

void SettingsModel::setValue(const QString& tag, const QString& value){
    data.insert(tag, value);
    emit dataChanged(tag);
}

void SettingsModel::setValue(const QString& tag, const int value){
    data.insert(tag, QString::number(value));
    emit dataChanged(tag);
}

void SettingsModel::setValue(const QString& tag, const double value){
    data.insert(tag, QString::number(value));
    emit dataChanged(tag);
}

QString SettingsModel::getText(const QString& tag) const{
    return data[tag];
}

int SettingsModel::getInt(const QString& tag) const{
    return data[tag].toInt();
}

double SettingsModel::getDouble(const QString& tag) const{
    return data[tag].toDouble();
}
