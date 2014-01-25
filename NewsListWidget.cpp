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
        this->allItems.append(title);

        //gets array of group data
        QList<NewsItem> items = news.values(keys[i]);

        //through all news in day
        for(int j = items.length()-1; j >= 0 ; j--){
            NewsItemWidget* item = new NewsItemWidget(content);
            item->setNewsItem(items[j]);
            connect(item, SIGNAL(pressed(NewsItemWidget*)), this, SLOT(itemPressed(NewsItemWidget*)));
            layout->addWidget(item);
            this->allItems.append(item);
            this->newsItems.append(item);
        }
    }

    //selects first item
    selectFirst();
}

void NewsListWidget::itemPressed(NewsItemWidget* item){
    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected())
            newsItems[i]->setSelected(false);
    }
    item->setSelected(true);
    emit pressed(item->newsItem());
}

void NewsListWidget::clearList(){
    for(int i = 0; i < allItems.length(); i++){
        layout->removeWidget(allItems[i]);
        delete allItems[i];
        allItems[i] = NULL;
    }

    allItems.clear();
    newsItems.clear();
}

NewsItem* NewsListWidget::selectFirst(){
    if(newsItems.isEmpty()) return NULL;

    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected()) newsItems[i]->setSelected(false);
    }

    newsItems.first()->setSelected(true);
    ensureWidgetVisible(newsItems.first());
    return newsItems.first()->newsItem();
}

NewsItem* NewsListWidget::selectNext(){
    if(newsItems.isEmpty()) return NULL;

    int selectedIndex = 0;

    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected()){
            newsItems[i]->setSelected(false);
            selectedIndex = i;
        }
    }

    if(selectedIndex+1 < newsItems.length()-1){
        newsItems[selectedIndex+1]->setSelected(true);
        ensureWidgetVisible(newsItems[selectedIndex+1]);
        return newsItems[selectedIndex+1]->newsItem();
    }else{
        newsItems.last()->setSelected(true);
        ensureWidgetVisible(newsItems.last());
        return newsItems.last()->newsItem();
    }
}

NewsItem* NewsListWidget::selectPrev(){
    if(newsItems.isEmpty()) return NULL;

    int selectedIndex = newsItems.length()-1;

    for(int i = newsItems.length()-1; i >= 0 ; i--){
        if(newsItems[i]->isSelected()){
            newsItems[i]->setSelected(false);
            selectedIndex = i;
        }
    }

    if(selectedIndex-1 > 0){
        newsItems[selectedIndex-1]->setSelected(true);
        ensureWidgetVisible(newsItems[selectedIndex-1]);
        return newsItems[selectedIndex-1]->newsItem();
    }else{
        newsItems.first()->setSelected(true);
        ensureWidgetVisible(newsItems.first());
        return newsItems.first()->newsItem();
    }
}

NewsItem* NewsListWidget::selectLast(){
    if(newsItems.isEmpty()) return NULL;

    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected()) newsItems[i]->setSelected(false);
    }

    newsItems.last()->setSelected(true);
    ensureWidgetVisible(newsItems.last());
    return newsItems.last()->newsItem();
}
