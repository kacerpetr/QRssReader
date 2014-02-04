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

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FeedManagement.h"
#include "StorageAccess.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"
#include "SettingsModel.h"

/**
 * @brief Class constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //hides line under message text
    ui->line1->setHidden(true);

    //hides progress bar
    ui->progressBar->setHidden(true);

    //tab actions
    actionGroup = new QActionGroup(ui->toolBar);
    actionGroup->setExclusive(true);
    actionGroup->addAction(ui->actionTab1);
    actionGroup->addAction(ui->actionTab2);
    actionGroup->addAction(ui->actionTab3);
    actionGroup->addAction(ui->actionTab4);
    actionGroup->addAction(ui->actionTab5);
    actionGroup->addAction(ui->actionTab6);
    actionGroup->addAction(ui->actionTab7);

    //loads settings
    SettingsModel::get().loadSettings();

    //cerates and inits rss models
    for(int i = 0; i < TAB_COUNT; i++){
        //creates data model
        RssDataModel* rssDataMdl = new RssDataModel(this);
        rssDataMdl->setFeedListFileName(RSS_FEED_FILE + QString::number(i+1) + ".xml");
        rssDataMdl->setCacheFolder(RSS_DATA_FOLDER + QString::number(i+1));

        //loads save data
        rssDataMdl->loadFeedList();
        rssDataMdl->loadRssCache();

        //signal slot connection
        //needs to be done after loadRssCache() because of dataChanged()
        connect(rssDataMdl, SIGNAL(dataChanged()), this, SLOT(updateNewsList()));
        connect(rssDataMdl, SIGNAL(loadingStarted(QString,int)), this, SLOT(updateProgressBar(QString,int)));
        connect(rssDataMdl, SIGNAL(loadingFinished()), this, SLOT(hideProgressBar()));

        //adds pointer to array
        rssData.append(rssDataMdl);
    }

    //creates and adds news list widgets to stack widget
    for(int i = 0; i < TAB_COUNT; i++){
        //creates news list
        NewsListWidget* newsListWg = new NewsListWidget(ui->newsListStack);

        //generates list from cache data
        newsListWg->clearList();
        newsListWg->createList(rssData[i]->data());

        //adds widget to stack
        ui->newsListStack->addWidget(newsListWg);

        //signal must be emited only when widget is current
        connect(newsListWg, SIGNAL(pressed(NewsItem*)), this, SLOT(itemPressed(NewsItem*)));
    }

    //sets width of news list
    settingsChanged("list_width");
    settingsChanged("view_footer_font_size");
    settingsChanged("view_text_font_size");
    settingsChanged("view_title_font_size");

    //selects first tab
    ui->actionTab1->setChecked(true);
    tabSelected(ui->actionTab1);

    //signal slot connection
    connect(&SettingsModel::get(), SIGNAL(dataChanged(QString)), this, SLOT(settingsChanged(QString)));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(ui->actionManageFeeds, SIGNAL(triggered()), this, SLOT(manageFeeds()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(refreshAction()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt5()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutApp()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(showAppHelp()));
    connect(ui->actionFirst, SIGNAL(triggered()), this, SLOT(selectFirst()));
    connect(ui->actionLast, SIGNAL(triggered()), this, SLOT(selectLast()));
    connect(ui->actionNext, SIGNAL(triggered()), this, SLOT(selectNext()));
    connect(ui->actionPrevious, SIGNAL(triggered()), this, SLOT(selectPrev()));
    connect(actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(tabSelected(QAction*)));
}

/**
 * @brief Class destructor
 */
MainWindow::~MainWindow(){
    delete ui;
}

/**
 * @brief Starts downloading of rss data
 */
void MainWindow::refreshAction(){
    rssDataCurrent->downloadRssData();
}

/**
 * @brief Called when item from news list is pressed
 * @param item pointer to selected news item
 */
void MainWindow::itemPressed(NewsItem* item){
    //unhides line under report text
    ui->line1->setHidden(false);

    //feed name and description
    QString name = "Feed: " + item->feed.name;
    if(!item->feed.description.isEmpty())
        name += " - " + item->feed.description;
    ui->feedNameLabel->setText(name);

    //report time
    ui->timeLabel->setText("Datetime: " +
    item->time.toString("ddd, dd MMM yyyy hh:mm:ss"));

    //link of full report
    QString linkText = item->link;
    if(item->link.length() > 50) linkText = item->link.left(20) + " ... " + item->link.right(20);
    ui->linkLabel->setText("Link: <a href=\""+item->link+"\">" + linkText + "</a>");

    //other report parts
    ui->titleLabel->setText(item->title);
    ui->textLabel->setText(item->text);
    ui->guidLabel->setText("Guid: " + item->guid);
}

/**
 * @brief Called when data was changed in RssDataModel
 */
void MainWindow::updateNewsList(){
    newsListCurrent->clearList();
    newsListCurrent->createList(rssDataCurrent->data());
    selectFirst();
}

/**
 * @brief Pregress bar update during rss data downloading
 * @param feed url of currently processed feed
 * @param progress in range from 0 to 100
 */
void MainWindow::updateProgressBar(QString feed, int progress){
    ui->progressBar->setHidden(false);
    ui->progressBar->setValue(progress);
    ui->progressBar->setFormat(feed);
}

/**
 * @brief Hides download progress bar
 */
void MainWindow::hideProgressBar(){
    ui->progressBar->setHidden(true);
}

/**
 * @brief Shows dialog about Qt5 framework
 */
void MainWindow::aboutQt5(){
    QMessageBox::aboutQt(this, "About Qt5");
}

/**
 * @brief Shows dialog about QRssReader
 */
void MainWindow::aboutApp(){
    QString about = "Application is currently in development state.\n";
    about += "by Petr Kacer <kacerpetr@gmail.com>";
    QMessageBox::about(this, "About QRssReader", about);
}

/**
 * @brief Shows feed management dialog
 */
void MainWindow::manageFeeds(){
    FeedManagement dialog;
    dialog.setModel(rssDataCurrent);
    #ifdef ANDROID
        dialog.setWindowState(Qt::WindowMaximized);
    #endif
    dialog.exec();
}

/**
 * @brief Shows settings dialog
 */
void MainWindow::showSettings(){
    SettingsDialog dialog;
    dialog.setRssModel(rssDataCurrent);
    #ifdef ANDROID
        dialog.setWindowState(Qt::WindowMaximized);
    #endif
    dialog.exec();
}

/**
 * @brief Shows application help
 */
void MainWindow::showAppHelp(){
    HelpDialog dialog;
    #ifdef ANDROID
        dialog.setWindowState(Qt::WindowMaximized);
    #endif
    dialog.exec();
}

/**
 * @brief Called when settings was changed
 * @param tag key of item in settings
 */
void MainWindow::settingsChanged(QString tag){
    if(tag == "list_width"){
        int width = SettingsModel::get().getInt(tag);
        ui->newsListStack->setMinimumWidth(width);
    }
    if(tag == "view_title_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setBold(true);
        font.setUnderline(true);
        font.setPointSize(sm.getInt(tag));
        ui->titleLabel->setFont(font);
    }
    if(tag == "view_text_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setPointSize(sm.getInt(tag));
        ui->textLabel->setFont(font);
    }
    if(tag == "view_footer_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setItalic(true);
        font.setPointSize(sm.getInt(tag));
        ui->timeLabel->setFont(font);
        ui->linkLabel->setFont(font);
        ui->feedNameLabel->setFont(font);
        ui->guidLabel->setFont(font);
    }
}

/**
 * @brief Selects first item from news list
 */
void MainWindow::selectFirst(){
    NewsItem* item = newsListCurrent->selectFirst();
    if(item == NULL) return;
    itemPressed(item);
}

/**
 * @brief Selects next item from news list
 */
void MainWindow::selectNext(){
    NewsItem* item = newsListCurrent->selectNext();
    if(item == NULL) return;
    itemPressed(item);
}

/**
 * @brief Selects previous item from news list
 */
void MainWindow::selectPrev(){
    NewsItem* item = newsListCurrent->selectPrev();
    if(item == NULL) return;
    itemPressed(item);
}

/**
 * @brief Selects last item from news list
 */
void MainWindow::selectLast(){
    NewsItem* item = newsListCurrent->selectLast();
    if(item == NULL) return;
    itemPressed(item);
}

/**
 * @brief Handles tab actions
 * @param action
 */
void MainWindow::tabSelected(QAction* action){
    //tab1 selected
    if(action == ui->actionTab1){
        rssDataCurrent = rssData[0];
        newsListCurrent = (NewsListWidget*)ui->newsListStack->widget(0);
        ui->newsListStack->setCurrentIndex(0);
    }
    //tab2 selected
    else if(action == ui->actionTab2){
        rssDataCurrent = rssData[1];
        newsListCurrent = (NewsListWidget*)ui->newsListStack->widget(1);
        ui->newsListStack->setCurrentIndex(1);
    }
    //tab3 selected
    else if(action == ui->actionTab3){
        rssDataCurrent = rssData[2];
        newsListCurrent = (NewsListWidget*)ui->newsListStack->widget(2);
        ui->newsListStack->setCurrentIndex(2);
    }
    //tab4 selected
    else if(action == ui->actionTab4){
        rssDataCurrent = rssData[3];
        newsListCurrent = (NewsListWidget*)ui->newsListStack->widget(3);
        ui->newsListStack->setCurrentIndex(3);
    }
    //tab5 selected
    else if(action == ui->actionTab5){
        rssDataCurrent = rssData[4];
        newsListCurrent = (NewsListWidget*)ui->newsListStack->widget(4);
        ui->newsListStack->setCurrentIndex(4);
    }
    //tab6 selected
    else if(action == ui->actionTab6){
        rssDataCurrent = rssData[5];
        newsListCurrent = (NewsListWidget*)ui->newsListStack->widget(5);
        ui->newsListStack->setCurrentIndex(5);
    }
    //tab7 selected
    else if(action == ui->actionTab7){
        rssDataCurrent = rssData[6];
        newsListCurrent = (NewsListWidget*)ui->newsListStack->widget(6);
        ui->newsListStack->setCurrentIndex(6);
    }

    //selected item
    NewsItem* item = newsListCurrent->selectedItem();
    //shows this item or selects first
    if(item != NULL) itemPressed(item);
    else selectFirst();
}
