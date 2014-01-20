#include "NewsList.h"
#include <QWidget>

NewsList::NewsList(QWidget *parent) : QScrollArea(parent){
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);

    content = new QWidget();
    content->setObjectName("NewsListContent");

    layout = new QVBoxLayout(content);
    layout->setMargin(0);
    layout->setSpacing(3);

    content->setLayout(layout);

    this->setWidget(content);
}

void NewsList::addItem(){
    for(int i = 0; i < 20; i++){
        NewsItem* item = new NewsItem(content);
        layout->addWidget(item);
    }
}
