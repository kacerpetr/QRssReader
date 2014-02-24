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
#include "FeedEditDialog.h"
#include "StorageAccess.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"
#include "SettingsModel.h"
#include "ActionBarWidget.h"

/**
 * @brief Class constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //creates additional widgets and adds them to layout
    finishUI();

    //loads settings
    SettingsModel::get().loadSettings();

    //creates and initializes data models
    createModels();

    //sets font sizes of news list
    newsViewWidget->settingsChanged("view_footer_font_size");
    newsViewWidget->settingsChanged("view_text_font_size");
    newsViewWidget->settingsChanged("view_title_font_size");

    //selects first tab
    ui->actionTab1->setChecked(true);
    tabSelected(ui->actionTab1);

    //action of left toolbar
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(showAppHelp()));
    connect(ui->actionFirst, SIGNAL(triggered()), this, SLOT(selectFirst()));
    connect(ui->actionLast, SIGNAL(triggered()), this, SLOT(selectLast()));
    connect(ui->actionNext, SIGNAL(triggered()), this, SLOT(selectNext()));
    connect(ui->actionPrevious, SIGNAL(triggered()), this, SLOT(selectPrev()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));

    //action of right toolbar
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(refreshAction()));
    connect(actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(tabSelected(QAction*)));
    connect(ui->actionManageFeeds, SIGNAL(triggered()), this, SLOT(manageFeeds()));   
}

/**
 * @brief Class destructor
 */
MainWindow::~MainWindow(){
    delete ui;
}

/**
 * @brief Creates additional widgets and adds them to layout
 */
void MainWindow::finishUI(){
    //hides progress bar
    ui->progressBar->setHidden(true);

    newsViewWidget = new NewsViewWidget(this);
    ui->leftVLayout->insertWidget(0, newsViewWidget);

    //tab actions
    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(ui->actionTab1);
    actionGroup->addAction(ui->actionTab2);
    actionGroup->addAction(ui->actionTab3);
    actionGroup->addAction(ui->actionTab4);
    actionGroup->addAction(ui->actionTab5);
    actionGroup->addAction(ui->actionTab6);
    actionGroup->addAction(ui->actionTab7);

    //top left action bar
    ActionBarWidget* leftActionBar = new ActionBarWidget(this);
    leftActionBar->setMargin(0,0,11,0);
    leftActionBar->setBackgroundColor(QColor(90,90,90));
    leftActionBar->addAction(ui->actionHelp, AlignLeft);
    leftActionBar->addAction(ui->actionFirst, AlignCenter);
    leftActionBar->addAction(ui->actionPrevious, AlignCenter);
    leftActionBar->addAction(ui->actionNext, AlignCenter);
    leftActionBar->addAction(ui->actionLast, AlignCenter);
    leftActionBar->addAction(ui->actionSettings, AlignRight);
    ui->leftVLayout->insertWidget(0, leftActionBar);

    //top right action bar
    ActionBarWidget* rightActionBar = new ActionBarWidget(this);
    rightActionBar->setBackgroundColor(QColor(70,70,70));
    rightActionBar->addAction(ui->actionRefresh, AlignLeft);
    rightActionBar->addAction(ui->actionTab1, AlignCenter);
    rightActionBar->addAction(ui->actionTab2, AlignCenter);
    rightActionBar->addAction(ui->actionTab3, AlignCenter);
    rightActionBar->addAction(ui->actionTab4, AlignCenter);
    rightActionBar->addAction(ui->actionTab5, AlignCenter);
    rightActionBar->addAction(ui->actionTab6, AlignCenter);
    rightActionBar->addAction(ui->actionTab7, AlignCenter);
    rightActionBar->addAction(ui->actionManageFeeds, AlignRight);
    ui->rightVLayout->insertWidget(0, rightActionBar);
}

/**
 * @brief Creates and initializes data models
 */
void MainWindow::createModels(){
    //cerates and inits rss models
    for(int i = 0; i < TAB_COUNT; i++){
        //creates data model
        RssDataModel* rssDataMdl = new RssDataModel(this);
        rssDataMdl->setFeedListFileName(RSS_FEED_FILE + QString::number(i+1) + ".xml");
        rssDataMdl->setCacheFolder(RSS_DATA_FOLDER + QString::number(i+1));
        rssDataMdl->setDefaultList(i+1);

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
        connect(newsListWg, SIGNAL(pressed(NewsItem*)), newsViewWidget, SLOT(itemPressed(NewsItem*)));
    }
}

/**
 * @brief Starts downloading of rss data
 */
void MainWindow::refreshAction(){
    rssDataCurrent->downloadRssData();
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
 * @brief Shows feed management dialog
 */
void MainWindow::manageFeeds(){
    FeedEditDialog dialog;
    dialog.setModels(&rssData);
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
 * @brief Selects first item from news list
 */
void MainWindow::selectFirst(){
    NewsItem* item = newsListCurrent->selectFirst();
    if(item == NULL) return;
    newsViewWidget->itemPressed(item);
}

/**
 * @brief Selects next item from news list
 */
void MainWindow::selectNext(){
    NewsItem* item = newsListCurrent->selectNext();
    if(item == NULL) return;
    newsViewWidget->itemPressed(item);
}

/**
 * @brief Selects previous item from news list
 */
void MainWindow::selectPrev(){
    NewsItem* item = newsListCurrent->selectPrev();
    if(item == NULL) return;
    newsViewWidget->itemPressed(item);
}

/**
 * @brief Selects last item from news list
 */
void MainWindow::selectLast(){
    NewsItem* item = newsListCurrent->selectLast();
    if(item == NULL) return;
    newsViewWidget->itemPressed(item);
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
    if(item != NULL) newsViewWidget->itemPressed(item);
    else selectFirst();
}
