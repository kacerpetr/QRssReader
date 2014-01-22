#include "NewsGroupWidget.h"
#include "ui_NewsGroupWidget.h"
#include <QPainter>

NewsGroupWidget::NewsGroupWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NewsGroupWidget){
    ui->setupUi(this);
}

void NewsGroupWidget::setText(QString text){
    ui->textLabel->setText(text);
}

void NewsGroupWidget::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

NewsGroupWidget::~NewsGroupWidget(){
    delete ui;
}

