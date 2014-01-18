#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include "TreeViewModel.h"

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

    private:
        Ui::MainWindow* ui;
        TreeViewModel* twm;

};

#endif // MAINWINDOW_H
