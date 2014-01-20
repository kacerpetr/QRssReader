#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include "FeedManagement.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    newsList = new NewsList(ui->newsListFrame);
    ui->newsListFrame->layout()->addWidget(newsList);

    newsView = new NewsView(ui->newsViewFrame);
    ui->newsViewFrame->layout()->addWidget(newsView);

    newsList->addItem();

    //twm = new TreeViewModel(ui->newsList);
    //ui->newsList->setModel(twm);

    connect(ui->ActionManageFeeds, SIGNAL(pressed()), this, SLOT(manageFeeds()));

    /*QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://idnes.cz.feedsportal.com/c/34387/f/625943/index.rss")));*/
}

void MainWindow::replyFinished(QNetworkReply* reply){
    qDebug() << reply->readAll();
}

void MainWindow::manageFeeds(){
    FeedManagement dialog;
    dialog.setWindowState(Qt::WindowMaximized);
    dialog.exec();
}

MainWindow::~MainWindow(){
    delete ui;
}
