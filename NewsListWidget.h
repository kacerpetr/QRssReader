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

    private:
        QWidget* content;
        QVBoxLayout* layout;
};

#endif // NEWSLISTWIDGET_H
