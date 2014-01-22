#include "NewsListWidget.h"
#include <QWidget>

NewsListWidget::NewsListWidget(QWidget *parent) : QScrollArea(parent){
    //scroll area settings
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);

    //scroll area content widget
    content = new QWidget(this);
    content->setObjectName("NewsListContent");

    //layout for scroll area content
    layout = new QVBoxLayout(content);
    layout->setMargin(0);
    layout->setSpacing(3);

    //puts it together
    content->setLayout(layout);
    this->setWidget(content);
}

void NewsListWidget::createList(const QMultiMap<QDate,NewsItem>& news){
    //gets array of keys (daily groups)
    QList<QDate> keys = news.uniqueKeys();

    //through daily groups
    for(int i = 0; i < keys.length(); i++){
        //adds group header
        NewsGroupWidget* header = new NewsGroupWidget(content);
        layout->addWidget(header);

        //gets array of group data
        QList<NewsItem> items = news.values(keys[i]);

        //through all news in day
        for(int j = 0; j < items.length(); j++){
            NewsItemWidget* item = new NewsItemWidget(content);
            layout->addWidget(item);
        }
    }
}

void NewsListWidget::clearList(){

}
