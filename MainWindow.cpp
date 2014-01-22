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
    rssData = new RssDataModel();
    rssFeed = new RssFeedModel(RSS_FEED_FILE);

    //creates and adds news list widget
    newsList = new NewsListWidget(ui->newsListFrame);
    ui->newsListFrame->layout()->addWidget(newsList);

    //creates and adds news view widget
    newsView = new NewsViewWidget(ui->newsViewFrame);
    ui->newsViewFrame->layout()->addWidget(newsView);

    //signal slot connection
    connect(ui->actionManageFeeds, SIGNAL(triggered()), this, SLOT(manageFeeds()));

    //TODO - testing
    rssFeed->loadFeedList();
    newsList->createList(rssData->data());
}


/*QNetworkAccessManager* manager = new QNetworkAccessManager(this);
connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
manager->get(QNetworkRequest(QUrl("http://idnes.cz.feedsportal.com/c/34387/f/625943/index.rss")));*/

void MainWindow::replyFinished(QNetworkReply* reply){
    qDebug() << reply->readAll();
}

void MainWindow::manageFeeds(){
    FeedManagement dialog;
    dialog.setWindowState(Qt::WindowMaximized);
    dialog.setModel(rssFeed);
    dialog.exec();
}

MainWindow::~MainWindow(){
    delete ui;
    delete rssData;
    rssData = NULL;
}
