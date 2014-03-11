#include "NewsViewWidget.h"
#include "ui_NewsViewWidget.h"
#include "SettingsModel.h"
#include <QScroller>

/**
 * @brief Class constructor
 */
NewsViewWidget::NewsViewWidget(QWidget *parent) : QWidget(parent), ui(new Ui::NewsViewWidget){
    ui->setupUi(this);

    //finger (left mouse) scrolling
    QScroller::grabGesture(ui->newsView, QScroller::LeftMouseButtonGesture);
    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    sp.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.1);
    sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    sp.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    sp.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.1);
    sp.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
    QScroller* qs = QScroller::scroller(ui->newsView);
    qs->setScrollerProperties(sp);

    //settings change handling
    connect(&SettingsModel::get(), SIGNAL(dataChanged(QString)), this, SLOT(settingsChanged(QString)));
}

/**
 * @brief Class destructor
 */
NewsViewWidget::~NewsViewWidget(){
    delete ui;
}

/**
 * @brief Called when item from news list is pressed
 * @param item pointer to selected news item
 */
void NewsViewWidget::itemPressed(TRssItem* item){
    //feed name and description
    QString name = "Feed: " + item->feed.name;
    if(!item->feed.desc.isEmpty())
        name += " - " + item->channel.desc;
    ui->feedNameLabel->setText(name);

    //report time
    ui->timeLabel->setText("Datetime: " +
    item->dt.toString("ddd, dd MMM yyyy hh:mm:ss"));

    //link of full report
    QString linkText = item->link;
    if(item->link.length() > 50) linkText = item->link.left(20) + " ... " + item->link.right(20);
    ui->linkLabel->setText("Link: <a href=\""+item->link+"\">" + linkText + "</a>");

    //other report parts
    ui->titleLabel->setText(item->title);
    ui->textLabel->setText(item->desc.trimmed());
    ui->guidLabel->setText("Guid: " + item->guid);

    //image label background color
    QString textClrStr = QString::number(item->feed.bkgColor.red()) + ",";
    textClrStr += QString::number(item->feed.bkgColor.green()) + ",";
    textClrStr += QString::number(item->feed.bkgColor.blue());
    textClrStr = "background: rgb(" + textClrStr + ");";
    ui->imageLabel->setStyleSheet(textClrStr);
}

/**
 * @brief Called when settings was changed
 * @param tag key of item in settings
 */
void NewsViewWidget::settingsChanged(QString tag){
    if(tag == "view_title_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setBold(true);
        font.setPointSize(sm.getInt(tag));
        ui->titleLabel->setFont(font);
    }
    else if(tag == "view_text_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setPointSize(sm.getInt(tag));
        ui->textLabel->setFont(font);
    }
    else if(tag == "view_footer_font_size"){
        SettingsModel& sm = SettingsModel::get();
        QFont font;
        font.setItalic(true);
        font.setPointSize(sm.getInt(tag));
        ui->timeLabel->setFont(font);
        ui->linkLabel->setFont(font);
        ui->feedNameLabel->setFont(font);
        ui->guidLabel->setFont(font);
    }
}
