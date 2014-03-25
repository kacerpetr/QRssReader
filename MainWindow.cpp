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
    connect(&SettingsModel::get(), SIGNAL(dataChanged(QString)), this, SLOT(settingsChanged(QString)));
    maxTabCount = SettingsModel::get().getInt("feedlist_tab_count");

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

    //hides and shows tab buttons according to max tab count from settings
    hideTabButtons();

    //resize news list
    QList<int> sizes;
    sizes.append(5000);
    sizes.append(4800);
    ui->splitter->setSizes(sizes);
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
    //leftActionBar->setMargin(0,0,11,0);
    leftActionBar->setBackgroundColor(QColor(90,90,90));
    leftActionBar->addAction(ui->actionHelp, AlignLeft);
    leftActionBar->addAction(ui->actionFirst, AlignCenter);
    leftActionBar->addAction(ui->actionPrevious, AlignCenter);
    leftActionBar->addAction(ui->actionNext, AlignCenter);
    leftActionBar->addAction(ui->actionLast, AlignCenter);
    leftActionBar->addAction(ui->actionSettings, AlignRight);
    ui->leftVLayout->insertWidget(0, leftActionBar);

    //top right action bar
    rightActionBar = new ActionBarWidget(this);
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
    //tab count
    int tabCount = SettingsModel::get().getInt("feedlist_tab_count");

    //data for app introduction, shown instead of empty newslist and news view
    introData = new RssDataModel(this);
    introData->setFeedListFileName(":/intro/intro_feedlist");
    introData->loadFeedList();
    introData->loadRssFile(":/intro/intro_content", 0);

    //cerates and inits rss models
    for(int i = 0; i < MAX_TAB_COUNT; i++){
        //creates data model
        RssDataModel* rssDataMdl = new RssDataModel(this);
        rssDataMdl->setFeedListFileName(RSS_FEED_FILE + QString::number(i+1) + ".xml");
        rssDataMdl->setCacheFolder(RSS_DATA_FOLDER + QString::number(i+1));
        rssDataMdl->setDefaultList(i+1);

        //loads save data
        if(i < tabCount){
            rssDataMdl->loadFeedList();
            rssDataMdl->loadRssCache();
        }

        //signal slot connection
        //needs to be done after loadRssCache() because of dataChanged()
        connect(rssDataMdl, SIGNAL(dataChanged()), this, SLOT(updateNewsList()));
        connect(rssDataMdl, SIGNAL(loadingStarted(QString,int)), this, SLOT(updateProgressBar(QString,int)));
        connect(rssDataMdl, SIGNAL(loadingFinished()), this, SLOT(hideProgressBar()));

        //adds pointer to array
        rssData.append(rssDataMdl);
    }

    //creates and adds news list widgets to stack widget
    for(int i = 0; i < MAX_TAB_COUNT; i++){
        //creates news list
        NewsListWidget* newsListWg = new NewsListWidget(ui->newsListStack);

        //generates list from cache data
        if(i < tabCount){
            newsListWg->clearList();
            if(rssData[i]->data().isEmpty())
                newsListWg->createList(introData->data(), true);
            else
                newsListWg->createList(rssData[i]->data());
        }

        //adds widget to stack
        ui->newsListStack->addWidget(newsListWg);

        //signal must be emited only when widget is current
        connect(newsListWg, SIGNAL(pressed(TRssItem*)), newsViewWidget, SLOT(itemPressed(TRssItem*)));
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
    TRssItem* item = newsListCurrent->selectFirst();
    if(item == NULL) return;
    newsViewWidget->itemPressed(item);
}

/**
 * @brief Selects next item from news list
 */
void MainWindow::selectNext(){
    TRssItem* item = newsListCurrent->selectNext();
    if(item == NULL) return;
    newsViewWidget->itemPressed(item);
}

/**
 * @brief Selects previous item from news list
 */
void MainWindow::selectPrev(){
    TRssItem* item = newsListCurrent->selectPrev();
    if(item == NULL) return;
    newsViewWidget->itemPressed(item);
}

/**
 * @brief Selects last item from news list
 */
void MainWindow::selectLast(){
    TRssItem* item = newsListCurrent->selectLast();
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
    TRssItem* item = newsListCurrent->selectedItem();
    //shows this item or selects first
    if(item != NULL) newsViewWidget->itemPressed(item);
    else selectFirst();
}

void MainWindow::hideTabButtons(){
    //tab count
    int tabCount = SettingsModel::get().getInt("feedlist_tab_count");

    //hides or shows action
    for(int i = 0; i < MAX_TAB_COUNT; i++){
        if(i == 0) rightActionBar->hideAction(ui->actionTab1, i >= tabCount); else
        if(i == 1) rightActionBar->hideAction(ui->actionTab2, i >= tabCount); else
        if(i == 2) rightActionBar->hideAction(ui->actionTab3, i >= tabCount); else
        if(i == 3) rightActionBar->hideAction(ui->actionTab4, i >= tabCount); else
        if(i == 4) rightActionBar->hideAction(ui->actionTab5, i >= tabCount); else
        if(i == 5) rightActionBar->hideAction(ui->actionTab6, i >= tabCount); else
        if(i == 6) rightActionBar->hideAction(ui->actionTab7, i >= tabCount);
    }

    //tells that currently checked tab will be hidden
    bool checkFirst = false;
    if(ui->actionTab2->isChecked() && tabCount < 2) checkFirst = true;
    if(ui->actionTab3->isChecked() && tabCount < 3) checkFirst = true;
    if(ui->actionTab4->isChecked() && tabCount < 4) checkFirst = true;
    if(ui->actionTab5->isChecked() && tabCount < 5) checkFirst = true;
    if(ui->actionTab6->isChecked() && tabCount < 6) checkFirst = true;
    if(ui->actionTab7->isChecked() && tabCount < 7) checkFirst = true;

    //checks first tab if currently checked tab will be hidden
    if(checkFirst){
        ui->actionTab1->setChecked(true);
        tabSelected(ui->actionTab1);
    }
}

void MainWindow::settingsChanged(QString tag){
    if(tag != "feedlist_tab_count") return;
    hideTabButtons();

    //tab count
    int tabCount = SettingsModel::get().getInt("feedlist_tab_count");

    //loads unloaded data
     if(tabCount > maxTabCount){
        for(int i = maxTabCount; i < tabCount; i++){
            rssData[i]->loadFeedList();
            rssData[i]->loadRssCache(true);
            QWidget* wg = ui->newsListStack->widget(i);
            ((NewsListWidget*)wg)->clearList();

            if(rssData[i]->data().isEmpty())
                ((NewsListWidget*)wg)->createList(introData->data(), true);
            else
                ((NewsListWidget*)wg)->createList(rssData[i]->data());
        }
        maxTabCount = tabCount;
    }
}
