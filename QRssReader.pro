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
    NewsViewWidget.cpp \
    FeedEditWidget.cpp \
    FeedEditDialog.cpp \
    AbstractParser.cpp \
    Rss10Parser.cpp \
    Rss20Parser.cpp

HEADERS  += MainWindow.h \
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
    globaldef.h \
    FeedEditWidget.h \
    FeedEditDialog.h \
    AbstractParser.h \
    Rss10Parser.h \
    Rss20Parser.h

FORMS    += MainWindow.ui \
    NewsGroupWidget.ui \
    NewsItemWidget.ui \
    HelpDialog.ui \
    SettingsDialog.ui \
    ActionBarWidget.ui \
    NewsViewWidget.ui \
    FeedEditWidget.ui \
    FeedEditDialog.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    Images.qrc \
    Default.qrc

OTHER_FILES += \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
