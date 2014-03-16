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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include "NewsListWidget.h"
#include "NewsViewWidget.h"
#include "RssDataModel.h"
#include "RssFeedModel.h"
#include "ActionBarWidget.h"
#include "globaldef.h"

namespace Ui{
    class MainWindow;
}

/**
 * @brief The MainWindow class
 * Main screen of application
 */
class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = 0);
        void finishUI();
        void createModels();
        void hideTabButtons();
        ~MainWindow();

    public slots:
        void manageFeeds();
        void refreshAction();
        void updateNewsList();
        void updateProgressBar(QString feed, int progress);
        void showAppHelp();
        void showSettings();
        void hideProgressBar();
        void selectFirst();
        void selectNext();
        void selectPrev();
        void selectLast();
        void tabSelected(QAction* action);
        void settingsChanged(QString tag);

    private:
        Ui::MainWindow* ui;
        NewsListWidget* newsListCurrent;
        NewsViewWidget* newsViewWidget;
        QList<RssDataModel*> rssData;
        RssDataModel* rssDataCurrent;
        QActionGroup* actionGroup;
        ActionBarWidget* rightActionBar;
        int maxTabCount;
};

#endif // MAINWINDOW_H
