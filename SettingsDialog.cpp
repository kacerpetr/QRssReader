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

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "SettingsDialog.h"
#include "SettingsModel.h"
#include "StorageAccess.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDirIterator>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent),
ui(new Ui::SettingsDialog), feedModel(NULL), dataModel(NULL){
    ui->setupUi(this);

    //button focus
    ui->saveButton->setFocus();

    //fills form with data
    ui->listWidthBox->setValue(SettingsModel::get().getInt("list_width"));
    ui->listTitleSizeBox->setValue(SettingsModel::get().getInt("list_title_font_size"));
    ui->listTimeFontSize->setValue(SettingsModel::get().getInt("list_time_font_size"));
    ui->viewTitleSize->setValue(SettingsModel::get().getInt("view_title_font_size"));
    ui->viewTextSize->setValue(SettingsModel::get().getInt("view_text_font_size"));
    ui->viewFooterSize->setValue(SettingsModel::get().getInt("view_footer_font_size"));

    //predefined feed lists
    QDirIterator it(FEEDS_PREFIX, QDirIterator::Subdirectories);
    while(it.hasNext()){
        ui->listWidget->addItem(it.next().remove(0, QString(FEEDS_PREFIX).length()));
    }
    ui->listWidget->setCurrentRow(0);

    //connects
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(ui->loadButton, SIGNAL(pressed()), this, SLOT(loadPresetPressed()));
    connect(ui->clearCacheButton, SIGNAL(pressed()), this, SLOT(clearCachePressed()));
}

SettingsDialog::~SettingsDialog(){
    delete ui;
}

void SettingsDialog::setRssFeedModel(RssFeedModel* model){
    feedModel = model;
}

void SettingsDialog::setRssDataModel(RssDataModel* model){
    dataModel = model;
}

void SettingsDialog::clearCachePressed(){
    //does nothing if no cache dir was given
    if(dataModel == NULL) return;

    //clears cache directory
    if(StorageAccess::get().clearDir(dataModel->dataFolder())){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Clear cache");
        msgBox.setText("Rss data cache succesfully cleared");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void SettingsDialog::savePressed(){
    SettingsModel& sm = SettingsModel::get();

    if(ui->listWidthBox->value() != sm.getInt("list_width"))
        sm.setValue("list_width", ui->listWidthBox->value());

    if(ui->listTitleSizeBox->value() != sm.getInt("list_title_font_size"))
        sm.setValue("list_title_font_size", ui->listTitleSizeBox->value());

    if(ui->listTimeFontSize->value() != sm.getInt("list_time_font_size"))
        sm.setValue("list_time_font_size", ui->listTimeFontSize->value());

    if(ui->viewTitleSize->value() != sm.getInt("view_title_font_size"))
        sm.setValue("view_title_font_size", ui->viewTitleSize->value());

    if(ui->viewTextSize->value() != sm.getInt("view_text_font_size"))
        sm.setValue("view_text_font_size", ui->viewTextSize->value());

    if(ui->viewFooterSize->value() != sm.getInt("view_footer_font_size"))
        sm.setValue("view_footer_font_size", ui->viewFooterSize->value());

    //saves changed data
    sm.saveSettings();

    //closes dialog window
    close();
}

void SettingsDialog::loadPresetPressed(){
    //must have data models
    if(dataModel == NULL) return;
    if(feedModel == NULL) return;

    //storage access singleton reference
    StorageAccess& sa = StorageAccess::get();

    //predefined feedlist path
    QString pflPath = FEEDS_PREFIX + ui->listWidget->currentItem()->text();

    //error handling vars
    bool success = false;

    //deletes old feedlist
    sa.rmFile(feedModel->feedListFile());

    //creates copy of predefined feedlist to current feedlist
    QFile source(pflPath);
    success = source.copy(sa.absPath(feedModel->feedListFile()));

    //error check
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Load preset error");
        msgBox.setText("Error creating copy of predefined feed list file");
        msgBox.setInformativeText(source.errorString());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    //sets file permissions
    QFile file(sa.absPath(feedModel->feedListFile()));
    success = file.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner |
                                  QFileDevice::ReadGroup | QFileDevice::ReadOther);

    //error check
    if(!success){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Load preset error");
        msgBox.setText("Unable to set feedlist file permissions");
        msgBox.setInformativeText(file.errorString());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    //clears data cache
    sa.clearDir(dataModel->dataFolder());

    //success
    QMessageBox msgBox;
    msgBox.setWindowTitle("Load preset");
    msgBox.setText("Predefined feedlist loaded");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
