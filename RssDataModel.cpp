#include "RssDataModel.h"

RssDataModel::RssDataModel(){
    NewsItem item;
    item.file = QString();
    item.header = QString("Nadpis 1");
    item.text = QString("Text 1");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(12, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 1");
    item.text = QString("Text 1");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(12, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 1");
    item.text = QString("Text 1");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(12, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 1");
    item.text = QString("Text 1");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(12, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 1");
    item.text = QString("Text 1");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(12, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 1");
    item.text = QString("Text 1");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(12, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 1");
    item.text = QString("Text 1");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(12, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 2");
    item.text = QString("Text 2");
    item.time = QDateTime(QDate(2014, 1, 12), QTime(15, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 3");
    item.text = QString("Text 3");
    item.time = QDateTime(QDate(2014, 1, 13), QTime(16, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 4");
    item.text = QString("Text 4");
    item.time = QDateTime(QDate(2014, 1, 13), QTime(17, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 4");
    item.text = QString("Text 4");
    item.time = QDateTime(QDate(2014, 1, 13), QTime(17, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 4");
    item.text = QString("Text 4");
    item.time = QDateTime(QDate(2014, 1, 13), QTime(17, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 4");
    item.text = QString("Text 4");
    item.time = QDateTime(QDate(2014, 1, 13), QTime(17, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 4");
    item.text = QString("Text 4");
    item.time = QDateTime(QDate(2014, 1, 13), QTime(17, 55, 25));
    addNewsItem(item);

    item.file = QString();
    item.header = QString("Nadpis 4");
    item.text = QString("Text 4");
    item.time = QDateTime(QDate(2014, 1, 13), QTime(17, 55, 25));
    addNewsItem(item);
}

void RssDataModel::addNewsItem(const NewsItem& item){
    news.insert(item.time.date(), item);
}

const QMultiMap<QDate,NewsItem>& RssDataModel::data() const{
    return news;
}

