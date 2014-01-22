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
    FeedManagement.cpp \
    NewsGroupWidget.cpp \
    NewsItemWidget.cpp \
    NewsListWidget.cpp \
    NewsViewWidget.cpp \
    RssDataModel.cpp \
    StorageAccess.cpp \
    RssFeedModel.cpp

HEADERS  += MainWindow.h \
    FeedManagement.h \
    NewsGroupWidget.h \
    NewsItemWidget.h \
    NewsListWidget.h \
    NewsViewWidget.h \
    RssDataModel.h \
    StorageAccess.h \
    RssFeedModel.h

FORMS    += MainWindow.ui \
    FeedManagement.ui \
    NewsGroupWidget.ui \
    NewsItemWidget.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD
