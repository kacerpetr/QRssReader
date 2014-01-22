#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>

#include "NewsListWidget.h"
#include "NewsViewWidget.h"
#include "RssDataModel.h"
#include "RssFeedModel.h"

#define RSS_FEED_FILE "feedlist.xml"

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = 0);
        ~MainWindow();

    public slots:
        void replyFinished(QNetworkReply* reply);
        void manageFeeds();

    private:
        Ui::MainWindow* ui;
        NewsListWidget* newsList;
        NewsViewWidget* newsView;
        RssDataModel* rssData;
        RssFeedModel* rssFeed;
};

#endif // MAINWINDOW_H
