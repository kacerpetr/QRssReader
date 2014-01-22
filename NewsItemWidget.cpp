#include "NewsItemWidget.h"
#include "ui_NewsItemWidget.h"
#include <QPainter>
#include <QDebug>

NewsItemWidget::NewsItemWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NewsItemWidget){
    ui->setupUi(this);
}

void NewsItemWidget::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NewsItemWidget::setNewsItem(NewsItem item){
    //set labels
    ui->titleLabel->setText(item.header);
    ui->feedLabel->setText(item.time.time().toString("hh:mm:ss") + " - " + item.feed.name);

    //set colors
    setColor(item.feed.bkgColor, item.feed.textColor);

    //stores news item
    this->item = item;
}

void NewsItemWidget::setColor(QColor bkgColor, QColor textColor){
    //background color
    QString textClrStr = QString::number(textColor.red()) + ",";
    textClrStr += QString::number(textColor.green()) + ",";
    textClrStr += QString::number(textColor.blue());

    //text color
    QString bkgClrStr = QString::number(bkgColor.red()) + ",";
    bkgClrStr += QString::number(bkgColor.green()) + ",";
    bkgClrStr += QString::number(bkgColor.blue());

    //text color
    QString styleSheet = QString(
        "#titleLabel{ \n"
        "    font-size: 15pt; \n"
        "    font-weight: bold; \n"
        "} \n"
        "#feedLabel{ \n"
        "    font-size: 13pt; \n"
        "} \n"
        "#NewsItemWidget{ \n"
        "    background-color: rgb(" + bkgClrStr + "); \n"
        "    color: rgb(" + textClrStr + "); \n"
        "} \n"
    );

    //sets stylesheet
    setStyleSheet(styleSheet);
}

void NewsItemWidget::mousePressEvent(QMouseEvent*){
    emit pressed(&item);
}

NewsItemWidget::~NewsItemWidget(){
    delete ui;
}

