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
    RssDataModel.cpp \
    StorageAccess.cpp \
    RssFeedModel.cpp \
    HelpDialog.cpp \
    SettingsModel.cpp \
    SettingsDialog.cpp \
    ActionBarWidget.cpp \
    NewsViewWidget.cpp

HEADERS  += MainWindow.h \
    FeedManagement.h \
    NewsGroupWidget.h \
    NewsItemWidget.h \
    NewsListWidget.h \
    RssDataModel.h \
    StorageAccess.h \
    RssFeedModel.h \
    HelpDialog.h \
    SettingsModel.h \
    SettingsDialog.h \
    ActionBarWidget.h \
    NewsViewWidget.h \
    globaldef.h

FORMS    += MainWindow.ui \
    FeedManagement.ui \
    NewsGroupWidget.ui \
    NewsItemWidget.ui \
    HelpDialog.ui \
    SettingsDialog.ui \
    ActionBarWidget.ui \
    NewsViewWidget.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    Icons.qrc \
    Default.qrc

OTHER_FILES += \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
