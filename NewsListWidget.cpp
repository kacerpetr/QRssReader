#include "NewsListWidget.h"
#include <QWidget>
#include <QScrollBar>

NewsListWidget::NewsListWidget(QWidget *parent) : QScrollArea(parent){
    //scroll area settings
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //scroll area content widget
    content = new QWidget(this);
    content->setObjectName("NewsListContent");

    //layout for scroll area content
    layout = new QVBoxLayout(content);
    layout->setMargin(0);
    layout->setSpacing(1);

    //puts it together
    content->setLayout(layout);
    this->setWidget(content);
}

void NewsListWidget::createList(const QMultiMap<QDate,NewsItem>& news){
    //gets array of keys (daily groups)
    QList<QDate> keys = news.uniqueKeys();

    //through daily groups
    for(int i = keys.length()-1; i >=0; i--){
        //adds group header
        NewsGroupWidget* title = new NewsGroupWidget(content);
        title->setText(keys[i].toString("dddd dd.MM.yyyy"));
        layout->addWidget(title);
        this->items.append(title);

        //gets array of group data
        QList<NewsItem> items = news.values(keys[i]);

        //through all news in day
        for(int j = items.length()-1; j >= 0 ; j--){
            NewsItemWidget* item = new NewsItemWidget(content);
            item->setNewsItem(items[j]);
            connect(item, SIGNAL(pressed(NewsItem*)), this, SLOT(itemPressed(NewsItem*)));
            layout->addWidget(item);
            this->items.append(item);
        }
    }
}

void NewsListWidget::itemPressed(NewsItem* item){
    emit pressed(item);
}

void NewsListWidget::clearList(){
    for(int i = 0; i < items.length(); i++){
        layout->removeWidget(items[i]);
        delete items[i];
        items[i] = NULL;
    }
    items.clear();
}
