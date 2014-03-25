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

#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include "StorageAccess.h"

/**
 * @brief Returns reference of model object
 */
StorageAccess& StorageAccess::get(){
    static StorageAccess instance;
    return instance;
}

/**
 * @brief Returns name of application data directory
 */
QString StorageAccess::appDir() const{
    //data directory path
    QString path;

    //for android is temporarily used "/sdcard" - TODO
    //QStandardPaths used for getting home folder is used
    #ifdef ANDROID
        path = "/sdcard";
    #else
        path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    #endif

    //creates QDir at home dir / android storage
    QDir dataDir(path);

    //checks if application directory exists in data dir
    //if not, application directory will be created
    if(!dataDir.exists(APP_FOLDER)){
        if(!dataDir.mkdir(APP_FOLDER)){
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error creating application directory");
            msgBox.setText("Error when calling dataDir.mkdir(APP_FOLDER)");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    }

    //returns absolute path of application directory
    return dataDir.absoluteFilePath(APP_FOLDER);
}

/**
 * @brief Returns absolute path of given file
 * Given file pathname is relative to application directory
 */
QString StorageAccess::absPath(QString filename) const{
    //absolute path of application directory
    QDir directory(appDir());

    //absolute path of file in app directory
    return directory.absoluteFilePath(filename);
}

/**
 * @brief Returns QXmlStreamReader that will read from given file
 * Given file pathname is relative to application directory
 */
QXmlStreamReader* StorageAccess::openXmlReader(QString filename) const{
    //opens xml file
    QString path = absPath(filename);

    //file object allocation
    QFile* file = new QFile(path);
    if(file == NULL){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in openXmlReader(...)");
        msgBox.setText("QFile object allocation error");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return NULL;
    }

    //opens file
    bool success = file->open(QIODevice::ReadOnly);

    //in case of failure
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in openXmlReader(...)");
        msgBox.setText("File \"" + path + "\" can't be opened (for reading)");
        msgBox.setInformativeText("Check if file exists or program have permissions to read it.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        delete file;
        return NULL;
    }

    //reader object allocation
    QXmlStreamReader* reader = new QXmlStreamReader(file);
    if(reader == NULL){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in openXmlReader(...)");
        msgBox.setText("QXmlStreamReader object allocation error");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        delete file;
        return NULL;
    }

    //returns reader object
    return reader;
}

/**
 * @brief Closes given QXmlStreamReader
 * @param reader
 */
void StorageAccess::closeXmlReader(QXmlStreamReader** reader) const{
    //parsing error check
    if((*reader)->hasError()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Open xml file");
        msgBox.setText("Error when parsing xml file.");
        msgBox.setInformativeText((*reader)->errorString());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    //closes file
    QIODevice* device = (*reader)->device();
    (*reader)->clear();
    device->close();
    delete device;
    device = NULL;

    //deletes reader
    delete *reader;
    *reader = NULL;
}

/**
 * @brief Returns QXmlStreamWriter that will write into given file
 * Given file pathname is relative to application directory
 */
QXmlStreamWriter* StorageAccess::openXmlWriter(QString filename) const{
    //opens xml file
    QString path = absPath(filename);

    //file object allocation
    QFile* file = new QFile(path);
    if(file == NULL){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in openXmlWriter(...)");
        msgBox.setText("QFile object allocation error");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return NULL;
    }

    //opens file
    bool success = file->open(QIODevice::WriteOnly);

    //in case of failure
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in openXmlWriter(...)");
        msgBox.setText("File \"" + path + "\" can't be opened (for writing)");
        msgBox.setInformativeText("Check if program have permissions to write it.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        delete file;
        return NULL;
    }

    //writer object allocation
    QXmlStreamWriter* writer = new QXmlStreamWriter(file);
    if(writer == NULL){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in openXmlWriter(...)");
        msgBox.setText("QXmlStreamWriter object allocation error");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        delete file;
        return NULL;
    }

    //returns writer object
    return writer;
}

/**
 * @brief Closes given QXmlStreamWriter
 * @param writer
 */
void StorageAccess::closeXmlWriter(QXmlStreamWriter** writer) const{
    //parsing error check
    if((*writer)->hasError()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Save xml file");
        msgBox.setText("Error when writing xml file.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    //closes file
    QIODevice* device = (*writer)->device();
    (*writer)->setDevice(NULL);
    delete device;
    device = NULL;

    //deletes reader
    delete *writer;
    *writer = NULL;
}

/**
 * @brief Writes given string to file
 * @param str string to write into file
 * @param relativePath output file pathname
 * @return true when success, false otherwise
 */
bool StorageAccess::writeString(const QString& str, const QString& relativePath) const{
    //absolute path of file
    QString absolutePath = absPath(relativePath);

    //opens file
    QFile file(absolutePath);
    bool success = file.open(QIODevice::WriteOnly);

    //in case of failure
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in writeString(...)");
        msgBox.setText("File \"" + absolutePath + "\" can't be opened (for writing)");
        msgBox.setInformativeText("Check if program have permissions to write it.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    //creates stream and writes file
    QTextStream outStream(&file);
    outStream << str;

    //QFile destructor will do it too
    file.close();

    //success
    return true;
}

/**
 * @brief Reads text file
 * @param result output variable
 * @param relativePath input file pathname
 * @return true when success, false otherwise
 */
bool StorageAccess::readString(QString& result, const QString& relativePath) const{
    //absolute path of file
    QString absolutePath = absPath(relativePath);

    //opens file
    QFile file(absolutePath);
    bool success = file.open(QIODevice::ReadOnly);

    //in case of failure
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in readString(...)");
        msgBox.setText("File \"" + absolutePath + "\" can't be opened (for reading)");
        msgBox.setInformativeText("Check if file exists or program have permissions to read it.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    //creates stream and writes file
    QTextStream inStream(&file);
    result = inStream.readAll();

    //QFile destructor will do it too
    file.close();

    //success
    return true;
}

/**
 * @brief Creates directory
 * @param relativePath
 * @return true when success, false otherwise
 */
bool StorageAccess::mkDir(const QString& relativePath) const{
    //application directory
    QDir appQDir(appDir());

    //does nothing if directory exists
    if(appQDir.exists(relativePath)) return true;

    //creates directory if not
    if(!appQDir.mkdir(relativePath)){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in mkDir(...)");
        msgBox.setText("Directory \"" + relativePath + "\" can't be created");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    //success
    return true;
}

/**
 * @brief Returns true if file exists
 * @param relativePath
 */
bool StorageAccess::exists(const QString& relativePath) const{
    QDir appQDir(appDir());
    return appQDir.exists(relativePath);
}

/**
 * @brief Clears given directory
 * @param relativePath
 * @return true when success, false otherwise
 */
bool StorageAccess::clearDir(const QString& relativePath) const{
    //directory to clear
    QDir dir(absPath(relativePath));

    //error handleing vars
    QString errFile;
    bool success = true;

    //clears all files from directory
    dir.setFilter( QDir::NoDotAndDotDot | QDir::Files );
    foreach(QString item, dir.entryList()){
        success = dir.remove(item);
        if(!success){
            errFile = item;
            break;
        }
    }

    //creates directory if not
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error in clearDir(...)");
        msgBox.setText("Directory \"" + relativePath + "\" can't be cleared");
        msgBox.setInformativeText("File \"" + errFile + "\" can't be deleted");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    //everithing ok?
    return success;
}

/**
 * @brief Removes given file
 * @param relativePath
 * @return true when success, false otherwise
 */
bool StorageAccess::rmFile(const QString& relativePath) const{
    //application directory
    QDir appQDir(appDir());

    //deletes file
    if(!appQDir.remove(relativePath)){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error rmFile(...)");
        msgBox.setText("File \"" + relativePath + "\" can't be deleted");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    return true;
}
