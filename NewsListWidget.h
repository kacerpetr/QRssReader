#ifndef NEWSLISTWIDGET_H
#define NEWSLISTWIDGET_H

#include <QScrollArea>
#include <QVBoxLayout>

#include "NewsItemWidget.h"
#include "NewsGroupWidget.h"
#include "RssDataModel.h"

class NewsListWidget : public QScrollArea{
    Q_OBJECT

    public:
        explicit NewsListWidget(QWidget* parent = 0);
        void createList(const QMultiMap<QDate,NewsItem>& news);
        void clearList();

    signals:
        void pressed(NewsItem* item);

    private slots:
        void itemPressed(NewsItem* item);

    private:
        QWidget* content;
        QVBoxLayout* layout;
        QList<QWidget*> items;
};

#endif // NEWSLISTWIDGET_H