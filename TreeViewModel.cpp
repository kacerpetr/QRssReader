#include "TreeViewModel.h"
#include <QDebug>

TreeViewModel::TreeViewModel(QObject* parent) : QAbstractItemModel(parent){
    NewsItem item;
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
}

void TreeViewModel::addNewsItem(const NewsItem& item){
    news.insert(item.time.date(), item);
}

QModelIndex TreeViewModel::index(int row, int column, const QModelIndex& parent) const{
    //no parent -> group index will be created
    if(!parent.isValid()){
        QDate key = news.uniqueKeys()[row];
        return createIndex(row, column, dateToNum(key));
    }

    //parent is group -> item index will be created
    if(isGroup(parent)){
        return createIndex(row, column, parent.internalId()+row+1);
    }

    //otherwise
    return QModelIndex();
}

QModelIndex TreeViewModel::parent(const QModelIndex& child) const{
    //invalid child or child is group
    if(!child.isValid() || isGroup(child)){
        return QModelIndex();
    }

    //child is item -> group index creation
    quint32 id = child.internalId() - child.internalId() % MAX_MSG_COUNT;
    return createIndex(news.keys().indexOf(toKey(child)), 1, id);
}

int TreeViewModel::rowCount(const QModelIndex& parent) const{
    if(!parent.isValid()) return news.uniqueKeys().length();
    if(isGroup(parent)) return news.count(toKey(parent));
    return 0;
}

int TreeViewModel::columnCount(const QModelIndex& parent) const{
    if(!parent.isValid()) return 1;
    return 1;
}

QVariant TreeViewModel::data(const QModelIndex& index, int role) const{
    //invalid index - no data
    if(!index.isValid()){
        return QVariant();
    }

    //data to display
    if(role == Qt::DisplayRole){
        if(isItem(index)){
            QList<NewsItem> items = news.values(toKey(index));
            return QVariant(items[toIndex(index)].header);
        }else{
            return QVariant("Den " + toKey(index).toString("dd.MM.yyyy"));
        }
    }

    //other options
    return QVariant();
}

bool TreeViewModel::isItem(const QModelIndex& index) const{
    if(!index.isValid()) return false;
    if(index.internalId() % MAX_MSG_COUNT == 0) return false;
    return true;
}

bool TreeViewModel::isGroup(const QModelIndex& index) const{
    if(!index.isValid()) return false;
    if(index.internalId() % MAX_MSG_COUNT != 0) return false;
    return true;
}

QDate TreeViewModel::toKey(const QModelIndex& index) const{
    quint32 date = index.internalId() / MAX_MSG_COUNT;
    return QDate(date/10000+CENTURY, (date/100)%100, date%100);
}

quint32 TreeViewModel::dateToNum(const QDate& date) const{
    return MAX_MSG_COUNT * (date.day() + date.month()*100 + (date.year()-CENTURY)*10000);
}

int TreeViewModel::toIndex(const QModelIndex& index) const{
    if(!isItem(index)) return 0;
    return index.internalId() % MAX_MSG_COUNT - 1;
}
