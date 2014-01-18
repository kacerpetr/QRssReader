#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), twm(NULL){
    ui->setupUi(this);
    twm = new TreeViewModel(ui->newsList);
    ui->newsList->setModel(twm);

    /*QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://idnes.cz.feedsportal.com/c/34387/f/625943/index.rss")));*/
}

void MainWindow::replyFinished(QNetworkReply* reply){
    qDebug() << reply->readAll();
}

MainWindow::~MainWindow(){
    delete ui;
}
