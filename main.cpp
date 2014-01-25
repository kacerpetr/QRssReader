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

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
