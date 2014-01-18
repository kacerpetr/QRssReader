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
    RssData.cpp \
    TreeViewModel.cpp

HEADERS  += MainWindow.h \
    RssData.h \
    TreeViewModel.h

FORMS    += MainWindow.ui

CONFIG += mobility
MOBILITY = 

