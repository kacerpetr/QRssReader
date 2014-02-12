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

#include "FeedManagement.h"
#include "ui_FeedManagement.h"
#include <QMessageBox>
#include <QClipboard>

/**
 * @brief Class constructor
 * @param parent
 */
FeedManagement::FeedManagement(QWidget* parent) : QDialog(parent), ui(new Ui::FeedManagement), model(NULL){
    ui->setupUi(this);

    //sets focus to save button
    ui->saveButton->setFocus();

    //background color preview
    connect(ui->bkgRedBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));
    connect(ui->bkgGreenBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));
    connect(ui->bkgBlueBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));

    //feed list selection
    connect(ui->feedList, SIGNAL(currentRowChanged(int)), this, SLOT(feedSelected(int)));

    //main control buttons
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(saveChangesPressed()));
    connect(ui->addButton, SIGNAL(pressed()), this, SLOT(addNewPressed()));
    connect(ui->removeButton, SIGNAL(pressed()), this, SLOT(removePressed()));

    //paste buttons
    connect(ui->pasteNameButton, SIGNAL(pressed()), this, SLOT(pasteNamePressed()));
    connect(ui->urlNameButton, SIGNAL(pressed()), this, SLOT(pasteUrlPressed()));
    connect(ui->descriptionNameButton, SIGNAL(pressed()), this, SLOT(pasteDescriptionPressed()));
}

/**
 * @brief Class destructor
 */
FeedManagement::~FeedManagement(){
    delete ui;
}

/**
 * @brief Sets RssFeedModel pointer
 * @param model
 * @see RssFeedModel
 */
void FeedManagement::setModel(RssFeedModel* model){
    //saves model pointer
    this->model = model;

    //list of feeds
    const QList<FeedItem>& items = model->feedList();

    //fills list widget
    for(int i = 0; i < items.length(); i++){
        ui->feedList->addItem(items[i].name + " - " + items[i].description);
    }

    //selects first item
    ui->feedList->setCurrentRow(0);
}

/**
 * @brief Returns FeedItem created from dialog inputs
 * @see FeedItem
 */
FeedItem FeedManagement::makeFeedItem() const{
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
void FeedManagement::feedSelected(int row){
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
void FeedManagement::bkgColorChanged(){
    QString color = QString::number(ui->bkgRedBox->value()) + ",";
    color += QString::number(ui->bkgGreenBox->value()) + ",";
    color += QString::number(ui->bkgBlueBox->value());
    ui->bkgColorPreview->setStyleSheet("background-color: rgb(" + color + ");");
}

/**
 * @brief Saves modified feed parameters as new feed
 */
void FeedManagement::addNewPressed(){
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
void FeedManagement::saveChangesPressed(){
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
void FeedManagement::removePressed(){
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
void FeedManagement::pasteNamePressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->nameEdit->setText(clipboard->text());
}

/**
 * @brief Pastes text from clipboard to feed url input
 */
void FeedManagement::pasteUrlPressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->urlEdit->setText(clipboard->text());
}

/**
 * @brief Pastes text from clipboard to feed description input
 */
void FeedManagement::pasteDescriptionPressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->descriptionEdit->setText(clipboard->text());
}
