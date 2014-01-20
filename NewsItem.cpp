#include "NewsItem.h"
#include "ui_NewsItem.h"
#include <QPainter>

NewsItem::NewsItem(QWidget *parent) : QWidget(parent), ui(new Ui::NewsItem){
    ui->setupUi(this);
}

void NewsItem::paintEvent(QPaintEvent *){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

NewsItem::~NewsItem(){
    delete ui;
}

