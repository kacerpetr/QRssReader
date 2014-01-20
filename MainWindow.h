#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include "NewsList.h"
#include "NewsView.h"

namespace Ui {
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
        NewsList* newsList;
        NewsView* newsView;
};

#endif // MAINWINDOW_H
