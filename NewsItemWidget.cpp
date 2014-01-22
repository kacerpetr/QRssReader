#include "NewsItemWidget.h"
#include "ui_NewsItemWidget.h"
#include <QPainter>

NewsItemWidget::NewsItemWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NewsItemWidget){
    ui->setupUi(this);
}

void NewsItemWidget::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

NewsItemWidget::~NewsItemWidget(){
    delete ui;
}

