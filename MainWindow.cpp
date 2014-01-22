#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FeedManagement.h"
#include "StorageAccess.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //cerates rss models
    rssFeed = new RssFeedModel(RSS_FEED_FILE);
    rssData = new RssDataModel(this);
    rssData->setFeedModel(rssFeed);

    //creates and adds news list widget
    newsList = new NewsListWidget(ui->newsListFrame);
    ui->newsListFrame->layout()->addWidget(newsList);

    //signal slot connection
    connect(ui->actionManageFeeds, SIGNAL(triggered()), this, SLOT(manageFeeds()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(refreshAction()));
    connect(newsList, SIGNAL(pressed(NewsItem*)), this, SLOT(itemPressed(NewsItem*)));
    connect(rssData, SIGNAL(dataChanged()), this, SLOT(updateNewsList()));

    //TODO - testing
    rssFeed->loadFeedList();
    newsList->createList(rssData->data());
}

void MainWindow::manageFeeds(){
    FeedManagement dialog;
    dialog.setWindowState(Qt::WindowMaximized);
    dialog.setModel(rssFeed);
    dialog.exec();
}

void MainWindow::refreshAction(){
    rssData->loadRssData();
}

void MainWindow::itemPressed(NewsItem* item){
    ui->titleLabel->setText(item->header);
    ui->textLabel->setText(item->text);

    QString name = item->feed.name;
    if(!item->feed.description.isEmpty())
        name += " - " + item->feed.description;
    ui->feedNameLabel->setText(name);

    ui->messageTimeLabel->setText(
    item->time.toString("ddd, dd MMM yyyy hh:mm:ss"));
}

void MainWindow::updateNewsList(){
    newsList->clearList();
    newsList->createList(rssData->data());
}

MainWindow::~MainWindow(){
    delete ui;
    delete rssFeed;
    rssFeed = NULL;
    delete rssData;
    rssData = NULL;
}
