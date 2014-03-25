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
#include <QFile>

/**
 * @brief Class constructor
 * @param parent
 */
SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent),
ui(new Ui::SettingsDialog), model(NULL){
    ui->setupUi(this);

    //fills form with data
    ui->listTitleSizeBox->setValue(SettingsModel::get().getInt("list_title_font_size"));
    ui->listTimeFontSize->setValue(SettingsModel::get().getInt("list_time_font_size"));
    ui->viewTitleSize->setValue(SettingsModel::get().getInt("view_title_font_size"));
    ui->viewTextSize->setValue(SettingsModel::get().getInt("view_text_font_size"));
    ui->viewFooterSize->setValue(SettingsModel::get().getInt("view_footer_font_size"));
    ui->feedlistTabCount->setValue(SettingsModel::get().getInt("feedlist_tab_count"));
    ui->maxViewedDays->setValue(SettingsModel::get().getInt("max_viewed_days"));

    //connects
    connect(ui->okToolButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(ui->clearCacheButton, SIGNAL(pressed()), this, SLOT(clearCachePressed()));


    //loads about text from resources
    #ifdef ANDROID
        QFile file(":/strings/about_android");
        file.open(QIODevice::ReadOnly);
        ui->aboutLabel->setText(file.readAll());
        file.close();
    #else
        QFile file(":/strings/about_pc");
        file.open(QIODevice::ReadOnly);
        ui->aboutLabel->setText(file.readAll());
        file.close();
    #endif
}

/**
 * @brief Class destructor
 */
SettingsDialog::~SettingsDialog(){
    delete ui;
}

/**
 * @brief RssDataModel setter
 * @param model
 */
void SettingsDialog::setRssModel(RssDataModel* model){
    this->model = model;
}

/**
 * @brief Clears rss data cache
 * Called when clear cache button pressed
 */
void SettingsDialog::clearCachePressed(){
    //does nothing if no cache dir was given
    if(model == NULL) return;

    //clears cache directory
    if(StorageAccess::get().clearDir(model->rssCacheFolder())){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Clear cache");
        msgBox.setText("Rss data cache succesfully cleared");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

/**
 * @brief Saves changes in application settings
 */
void SettingsDialog::savePressed(){
    SettingsModel& sm = SettingsModel::get();

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

    if(ui->feedlistTabCount->value() != sm.getInt("feedlist_tab_count"))
        sm.setValue("feedlist_tab_count", ui->feedlistTabCount->value());

    if(ui->maxViewedDays->value() != sm.getInt("max_viewed_days"))
        sm.setValue("max_viewed_days", ui->maxViewedDays->value());

    //saves changed data
    sm.saveSettings();

    //closes dialog window
    close();
}
