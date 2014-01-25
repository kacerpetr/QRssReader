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

#include "MainWindow.h"
#include <QApplication>

//------ TODO -----//
// 1. check for duplicity and validity when loading feedList
// 2. get data directory in android in better way
// 3. what RssDataModel should do when no RssFeedModel was given
// 4. NewsListWidget - better naming (items X this->items)
// 5. check internet connection
// 6. rename NewsItem.header to NewsItem.title
// 7. determine behavior when no folder given to RssDataModel
// 8. do something if required settings tag does not exists
// 9. refactor settings to configuration file
// 10. dont save empty files to cache (when no internet connection)
// 11. change of background or font color have effect only with reload
// 12. add android like scrolling

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
