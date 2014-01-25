#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NewsListWidget.h"
#include "RssDataModel.h"
#include "RssFeedModel.h"

#define RSS_FEED_FILE "feedlist.xml"
#define RSS_DATA_FOLDER "RssData"

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = 0);
        ~MainWindow();

    public slots:
        void manageFeeds();
        void refreshAction();
        void itemPressed(NewsItem* item);
        void updateNewsList();
        void updateProgressBar(QString feed, int progress);
        void aboutQt5();
        void showAppHelp();
        void aboutApp();
        void showSettings();
        void hideProgressBar();
        void settingsChanged(QString tag);

    private:
        Ui::MainWindow* ui;
        NewsListWidget* newsList;
        RssDataModel* rssData;
        RssFeedModel* rssFeed;
};

#endif // MAINWINDOW_H
