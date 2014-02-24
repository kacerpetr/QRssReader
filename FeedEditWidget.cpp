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

#include "FeedEditWidget.h"
#include "ui_FeedEditWidget.h"
#include "StorageAccess.h"
#include <QMessageBox>
#include <QClipboard>
#include <QDirIterator>

FeedEditWidget::FeedEditWidget(QWidget *parent) : QWidget(parent), ui(new Ui::FeedEditWidget){
    ui->setupUi(this);

    //fills predefined feed lists
    listFeedPresets();

    //sets focus to save button
    ui->saveButton->setFocus();

    //background color preview
    connect(ui->bkgRedBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));
    connect(ui->bkgGreenBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));
    connect(ui->bkgBlueBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));

    //feed list selection
    connect(ui->feedList, SIGNAL(currentRowChanged(int)), this, SLOT(feedSelected(int)));
    connect(ui->feedPresetList, SIGNAL(currentRowChanged(int)), this, SLOT(presetSelected(int)));

    //main control buttons
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(saveChangesPressed()));
    connect(ui->addButton, SIGNAL(pressed()), this, SLOT(addNewPressed()));
    connect(ui->removeButton, SIGNAL(pressed()), this, SLOT(removePressed()));
    connect(ui->loadFeedlistButton, SIGNAL(pressed()), this, SLOT(loadPresetPressed()));

    //paste buttons
    connect(ui->pasteNameButton, SIGNAL(pressed()), this, SLOT(pasteNamePressed()));
    connect(ui->urlNameButton, SIGNAL(pressed()), this, SLOT(pasteUrlPressed()));
    connect(ui->descriptionNameButton, SIGNAL(pressed()), this, SLOT(pasteDescriptionPressed()));

    //close button
    connect(ui->closeButton, SIGNAL(pressed()), this, SLOT(emitClosePressed()));
}

FeedEditWidget::~FeedEditWidget(){
    delete ui;
}

/**
 * @brief Lists predefined feedlists and fills feedPresetList
 */
void FeedEditWidget::listFeedPresets(){
    //reads predefined feedlists and their names and descriptions
    lists.clear();
    model->predefinedFeedlist(lists);

    //fills list widget
    for(int i = 0; i < lists.length(); i++){
        if(lists[i].name.isEmpty())
            ui->feedPresetList->addItem(lists[i].url);
        else
            ui->feedPresetList->addItem(lists[i].name);
    }

    //selects first item
    ui->feedPresetList->setCurrentRow(0);
}

/**
 * @brief Sets RssFeedModel pointer
 * @param model
 * @see RssFeedModel
 */
void FeedEditWidget::setModel(RssDataModel* model){
    //saves model pointer
    this->model = model;

    //fills listWidget of feed
    fillFeedList();
}

/**
 * @brief Fills list of current feeds
 */
void FeedEditWidget::fillFeedList(){
    //clears view
    ui->feedList->clear();

    //list of feeds
    const QList<FeedItem>& items = model->feedList();

    //fills list widget
    for(int i = 0; i < items.length(); i++){
        ui->feedList->addItem(items[i].name + " - " + items[i].description);
    }

    //selects first item
    ui->feedList->setCurrentRow(0);
    presetSelected(0);
}

/**
 * @brief Returns FeedItem created from dialog inputs
 * @see FeedItem
 */
FeedItem FeedEditWidget::makeFeedItem() const{
    FeedItem item;

    //main properties
    item.name = ui->nameEdit->text();
    item.url = ui->urlEdit->text();
    item.description = ui->descriptionEdit->text();
    item.enabled = ui->enableBox->isChecked();

    //background color in news list
    item.bkgColor = QColor(
        ui->bkgRedBox->value(),
        ui->bkgGreenBox->value(),
        ui->bkgBlueBox->value()
    );

    return item;
}

/**
 * @brief Fills form inputs with feed data
 * @param row
 */
void FeedEditWidget::feedSelected(int row){
    //invalid row check
    if(row < 0) return;

    //list of feeds
    const QList<FeedItem>& items = model->feedList();

    //sets base properties of feed
    ui->nameEdit->setText(items[row].name);
    ui->urlEdit->setText(items[row].url);
    ui->descriptionEdit->setText(items[row].description);
    ui->enableBox->setChecked(items[row].enabled);

    //background color in news list
    ui->bkgRedBox->setValue(items[row].bkgColor.red());
    ui->bkgGreenBox->setValue(items[row].bkgColor.green());
    ui->bkgBlueBox->setValue(items[row].bkgColor.blue());

    //renew color previews
    bkgColorChanged();
}

/**
 * @brief Set color of background color preview label
 */
void FeedEditWidget::bkgColorChanged(){
    QString color = QString::number(ui->bkgRedBox->value()) + ",";
    color += QString::number(ui->bkgGreenBox->value()) + ",";
    color += QString::number(ui->bkgBlueBox->value());
    ui->bkgColorPreview->setStyleSheet("background-color: rgb(" + color + ");");
}

/**
 * @brief Saves modified feed parameters as new feed
 */
void FeedEditWidget::addNewPressed(){
    FeedItem item = makeFeedItem();

    //adds feed
    if(!model->addFeed(item)){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error adding new feed");
        msgBox.setText("Feed with given name or url already exists");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    //adds item to feed list
    ui->feedList->addItem(item.name + " - " + item.description);

    //saves feed list
    model->saveFeedList();
}

/**
 * @brief Saves changes of selected feed
 */
void FeedEditWidget::saveChangesPressed(){
    //index of selected item
    int index = ui->feedList->currentRow();
    if(index < 0) return;

    //modified feed
    FeedItem feed = makeFeedItem();

    //modifies feed
    if(!model->modifyFeed(index, feed)){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error saving feed changes");
        msgBox.setText("Name/url was changed to name/url of another existing item");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    //modifies item in feed list
     QListWidgetItem* item = ui->feedList->currentItem();
     item->setText(feed.name + " - " + feed.description);

     //saves feed list
     model->saveFeedList();
}

/**
 * @brief Removes selected feed
 */
void FeedEditWidget::removePressed(){
    //index of selected item
    int index = ui->feedList->currentRow();
    if(index < 0) return;

    //removes item from feed list
    QListWidgetItem* item = ui->feedList->takeItem(index);
    delete item;
    item = NULL;

    //removes feed from models
    model->removeFeed(index);

    //saves feed list
    model->saveFeedList();
}

/**
 * @brief Pastes text from clipboard to feed name input
 */
void FeedEditWidget::pasteNamePressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->nameEdit->setText(clipboard->text());
}

/**
 * @brief Pastes text from clipboard to feed url input
 */
void FeedEditWidget::pasteUrlPressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->urlEdit->setText(clipboard->text());
}

/**
 * @brief Pastes text from clipboard to feed description input
 */
void FeedEditWidget::pasteDescriptionPressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->descriptionEdit->setText(clipboard->text());
}

/**
 * @brief Emits closePressed signal when close button is pressed
 */
void FeedEditWidget::emitClosePressed(){
    emit closePressed();
}


/**
 * @brief Loads predefined feedlist
 */
void FeedEditWidget::loadPresetPressed(){
    //must have data models
    if(model == NULL) return;

    //storage access singleton reference
    StorageAccess& sa = StorageAccess::get();

    //predefined feedlist path
    QString pflPath = lists[ui->feedPresetList->currentRow()].url;

    //error handling vars
    bool success = false;

    //deletes old feedlist
    sa.rmFile(model->feedListFileName());

    //creates copy of predefined feedlist to current feedlist
    QFile source(pflPath);
    success = source.copy(sa.absPath(model->feedListFileName()));

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
    #ifndef ANDROID
        QFile file(sa.absPath(model->feedListFileName()));
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
    #endif

    //clears data cache
    sa.clearDir(model->rssCacheFolder());

    //and finaly, loads feedlist into model
    model->loadFeedList();

    //selects first item
    fillFeedList();
}

void FeedEditWidget::presetSelected(int currentRow){
    QString text = lists[currentRow].description;
    if(text.isEmpty()) text = "Feedlist has no description.";
    ui->feedlistDescriptionText->setPlainText(text);
}
