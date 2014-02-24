/*
 * This file is part of QRssReader, rss reader for pc and android.
 * Copyright (C) 2014, Petr Kačer <kacerpetr@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NewsListWidget.h"
#include <QWidget>
#include <QScrollBar>
#include <QScroller>
#include <QEasingCurve>
#include <QResizeEvent>

/**
 * @brief Class constructor
 * @param parent
 */
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
    layout->setSpacing(0);

    //puts it together
    content->setLayout(layout);
    this->setWidget(content);

    //enables figer scrolling for android
    #ifdef ANDROID
        QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
        QScrollerProperties sp;
        sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
        sp.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
        sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.1);
        sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        sp.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        sp.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.1);
        sp.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
        QScroller* qs = QScroller::scroller(this);
        qs->setScrollerProperties(sp);
    #endif
}

/**
 * @brief Fills scroll area with item widgets
 * @param news reference to rss data
 */
void NewsListWidget::createList(const QMultiMap<QDate,NewsItem>& news){
    //gets array of keys (daily groups)
    QList<QDate> keys = news.uniqueKeys();

    //used for background colors
    bool odd = true;

    //through daily groups
    for(int i = keys.length()-1; i >=0; i--){
        //adds group header
        NewsGroupWidget* title = new NewsGroupWidget(content);
        title->setOdd(odd);
        odd = !odd;
        title->setText(keys[i].toString("dddd dd.MM.yyyy"));
        layout->addWidget(title);
        this->allItems.append(title);

        //gets array of group data
        QList<NewsItem> items = news.values(keys[i]);

        //through all news in day
        for(int j = items.length()-1; j >= 0 ; j--){
            if(!items[j].feed.enabled) continue;
            NewsItemWidget* item = new NewsItemWidget(content);
            item->setNewsItem(items[j]);

            item->setOdd(odd);
            odd = !odd;

            item->setIcon(items[j].feed.bkgColor, items.length()-j);

            connect(item, SIGNAL(pressed(NewsItemWidget*)), this, SLOT(itemPressed(NewsItemWidget*)));

            layout->addWidget(item);
            this->allItems.append(item);
            this->newsItems.append(item);
        }
    }
}

/**
 * @brief Called when some item widget is pressed
 * @param item pointer of pressed widget
 */
void NewsListWidget::itemPressed(NewsItemWidget* item){
    //unselects all selected items
    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected())
            newsItems[i]->setSelected(false);
    }
    //selects presed item
    item->setSelected(true);

    //emits signal
    emit pressed(item->newsItem());
}

/**
 * @brief Clears news list
 */
void NewsListWidget::clearList(){
    //reamoves widgets from layout and deletes them
    for(int i = 0; i < allItems.length(); i++){
        layout->removeWidget(allItems[i]);
        delete allItems[i];
        allItems[i] = NULL;
    }

    //clears pointer lists
    allItems.clear();
    newsItems.clear();
}

/**
 * @brief Selects first item in list
 * @return NewsItem pointer asociated with current item widget
 */
NewsItem* NewsListWidget::selectFirst(){
    if(newsItems.isEmpty()) return NULL;

    //unselects all selected items
    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected()) newsItems[i]->setSelected(false);
    }

    //selects first item and scrolls to it
    newsItems.first()->setSelected(true);
    ensureWidgetVisible(newsItems.first());

    //returns asociated NewsItem pointer
    return newsItems.first()->newsItem();
}

/**
 * @brief Selects next item in list
 * @return NewsItem pointer asociated with current item widget
 */
NewsItem* NewsListWidget::selectNext(){
    if(newsItems.isEmpty()) return NULL;

    //index of selected item
    int selectedIndex = 0;

    //unselects all selected items and get index of last of them
    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected()){
            newsItems[i]->setSelected(false);
            selectedIndex = i;
        }
    }

    //next item selection
    if(selectedIndex+1 < newsItems.length()-1){
        newsItems[selectedIndex+1]->setSelected(true);
        ensureWidgetVisible(newsItems[selectedIndex+1]);
        return newsItems[selectedIndex+1]->newsItem();
    }
    //next item is last
    else{
        newsItems.last()->setSelected(true);
        ensureWidgetVisible(newsItems.last());
        return newsItems.last()->newsItem();
    }
}

/**
 * @brief Selects previous item in list
 * @return NewsItem pointer asociated with current item widget
 */
NewsItem* NewsListWidget::selectPrev(){
    if(newsItems.isEmpty()) return NULL;

    //index of selected item
    int selectedIndex = newsItems.length()-1;

    //unselects all selected items and get index of first of them
    for(int i = newsItems.length()-1; i >= 0 ; i--){
        if(newsItems[i]->isSelected()){
            newsItems[i]->setSelected(false);
            selectedIndex = i;
        }
    }

    //previous item selection
    if(selectedIndex-1 > 0){
        newsItems[selectedIndex-1]->setSelected(true);
        ensureWidgetVisible(newsItems[selectedIndex-1]);
        return newsItems[selectedIndex-1]->newsItem();
    }
    //previous item is first
    else{
        newsItems.first()->setSelected(true);
        ensureWidgetVisible(newsItems.first());
        return newsItems.first()->newsItem();
    }
}

/**
 * @brief Selects last item in list
 * @return NewsItem pointer asociated with current item widget
 */
NewsItem* NewsListWidget::selectLast(){
    if(newsItems.isEmpty()) return NULL;

    //unselects all selected items
    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected()) newsItems[i]->setSelected(false);
    }

    //selects last item and scrolls to it
    newsItems.last()->setSelected(true);
    ensureWidgetVisible(newsItems.last());

    //returns asociated NewsItem pointer
    return newsItems.last()->newsItem();
}

/**
 * @brief Returns pointer of selected news item
 */
NewsItem* NewsListWidget::selectedItem() const{
    //search of selectend item
    for(int i = 0; i < newsItems.length(); i++){
        if(newsItems[i]->isSelected())
            return newsItems[i]->newsItem();
    }

    //nothing found
    return NULL;
}

void NewsListWidget::resizeEvent(QResizeEvent* event){
    for(int i = 0; i < newsItems.length(); i++){
        newsItems[i]->setMaximumWidth(event->size().width());
    }
    QScrollArea::resizeEvent(event);
}
