#include "MainWindow.h"
#include <QApplication>

//------ TODO -----//
// 1. check for duplicity and validity when loading feedList
// 2. get data directory in android in better way

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
