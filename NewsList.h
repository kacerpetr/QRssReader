#ifndef NEWSLIST_H
#define NEWSLIST_H

#include <QScrollArea>
#include <QVBoxLayout>
#include "NewsItem.h"

class NewsList : public QScrollArea{
    Q_OBJECT

    public:
        explicit NewsList(QWidget *parent = 0);
        void addItem();

    signals:

    public slots:

    private:
        QWidget* content;
        QVBoxLayout* layout;
};

#endif // NEWSLIST_H
