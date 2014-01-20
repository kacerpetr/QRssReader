#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T11:57:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRssReader
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TreeViewModel.cpp \
    FeedManagement.cpp \
    NewsItem.cpp \
    NewsList.cpp \
    NewsView.cpp

HEADERS  += MainWindow.h \
    TreeViewModel.h \
    FeedManagement.h \
    NewsItem.h \
    NewsList.h \
    NewsView.h

FORMS    += MainWindow.ui \
    FeedManagement.ui \
    NewsItem.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    Icons.qrc

