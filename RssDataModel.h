#ifndef RSSDATAMODEL_H
#define RSSDATAMODEL_H

#include <QString>
#include <QDateTime>
#include <QMultiMap>
#include <QColor>

typedef struct{
    QString header;
    QString text;
    QDateTime time;
    QString file;
    QString source;
    QColor color;
}NewsItem;

class RssDataModel{
    public:
        RssDataModel();
        void addNewsItem(const NewsItem& item);
        const QMultiMap<QDate,NewsItem>& data() const;

    private:
        QMultiMap<QDate,NewsItem> news;
};

#endif // RSSDATAMODEL_H
