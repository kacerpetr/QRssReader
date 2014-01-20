#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QDateTime>
#include <QDate>
#include <QMultiMap>

#define MAX_MSG_COUNT 1000
#define CENTURY 2000

typedef struct{
    QString header;
    QString text;
    QDateTime time;
    QString file;
}NewsItem;

class TreeViewModel : public QAbstractItemModel{
    Q_OBJECT

    public:
        explicit TreeViewModel(QObject* parent = 0);
        void addNewsItem(const NewsItem& item);
        int dayCount() const;
        int newsCount(const QDate& date) const;

    public:
        QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex& child) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    private:
        bool isItem(const QModelIndex& index) const;
        bool isGroup(const QModelIndex& index) const;
        QDate toKey(const QModelIndex& index) const;
        quint32 dateToNum(const QDate& date) const;
        int toIndex(const QModelIndex& index) const;

    private:
        QMultiMap<QDate,NewsItem> news;
};

#endif
