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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //hides line under message text
    ui->line1->setHidden(true);

    //hides progress bar
    ui->progressBar->setHidden(true);

    //cerates and inits rss models
    rssFeed = new RssFeedModel(RSS_FEED_FILE);
    rssData = new RssDataModel(this);
    rssData->setFolder(RSS_DATA_FOLDER);
    rssData->setFeedModel(rssFeed);

    //creates and adds news list widget
    newsList = new NewsListWidget(ui->newsListFrame);
    ui->newsListFrame->layout()->addWidget(newsList);

    //signal slot connection
    connect(ui->actionManageFeeds, SIGNAL(triggered()), this, SLOT(manageFeeds()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(refreshAction()));
    connect(newsList, SIGNAL(pressed(NewsItem*)), this, SLOT(itemPressed(NewsItem*)));
    connect(rssData, SIGNAL(dataChanged()), this, SLOT(updateNewsList()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt5()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutApp()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(showAppHelp()));
    connect(rssData, SIGNAL(loadingStarted(QString,int)), this, SLOT(updateProgressBar(QString,int)));
    connect(rssData, SIGNAL(loadingFinished()), this, SLOT(hideProgressBar()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(&SettingsModel::get(), SIGNAL(dataChanged(QString)), this, SLOT(settingsChanged(QString)));

    //loads settings
    SettingsModel::get().loadSettings();

    //sets width of news list
    settingsChanged("list_width");
    settingsChanged("view_footer_font_size");
    settingsChanged("view_text_font_size");
    settingsChanged("view_title_font_size");

    //loads saved data
    rssFeed->loadFeedList();
    rssData->loadRss();
}

void MainWindow::refreshAction(){
    rssData->downloadRssData();
}

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
    ui->titleLabel->setText(item->header);
    ui->textLabel->setText(item->text);
    ui->guidLabel->setText("Guid: " + item->guid);
}

void MainWindow::updateNewsList(){
    newsList->clearList();
    newsList->createList(rssData->data());
}

void MainWindow::updateProgressBar(QString feed, int progress){
    ui->progressBar->setHidden(false);
    ui->progressBar->setValue(progress);
    ui->progressBar->setFormat(feed);
}

void MainWindow::hideProgressBar(){
    ui->progressBar->setHidden(true);
}

void MainWindow::aboutQt5(){
    QMessageBox::aboutQt(this, "About Qt5");
}

void MainWindow::aboutApp(){
    QString about = "Application is currently in development state.\n";
    about += "by Petr Kacer <kacerpetr@gmail.com>";
    QMessageBox::about(this, "About QRssReader", about);
}

void MainWindow::manageFeeds(){
    FeedManagement dialog;
    #ifdef ANDROID
        dialog.setWindowState(Qt::WindowMaximized);
    #endif
    dialog.setModel(rssFeed);
    dialog.exec();
}

void MainWindow::showSettings(){
    SettingsDialog dialog;
    #ifdef ANDROID
        dialog.setWindowState(Qt::WindowMaximized);
    #endif
    dialog.exec();
}

void MainWindow::showAppHelp(){
    HelpDialog dialog;
    #ifdef ANDROID
        dialog.setWindowState(Qt::WindowMaximized);
    #endif
    dialog.exec();
}

void MainWindow::settingsChanged(QString tag){
    if(tag == "list_width"){
        int width = SettingsModel::get().getInt(tag);
        ui->newsListFrame->setMinimumWidth(width);
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

MainWindow::~MainWindow(){
    delete ui;
    delete rssFeed;
    rssFeed = NULL;
    delete rssData;
    rssData = NULL;
}
