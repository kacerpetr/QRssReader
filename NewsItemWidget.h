#ifndef NEWSITEM_H
#define NEWSITEM_H

#include <QWidget>
#include <QColor>
#include "RssDataModel.h"

namespace Ui {
    class NewsItemWidget;
}

class NewsItemWidget : public QWidget{
    Q_OBJECT

    public:
        explicit NewsItemWidget(QWidget* parent = 0);
        ~NewsItemWidget();
        void setNewsItem(NewsItem item);
        void setSelected(bool selected);
        NewsItem* newsItem();
        bool isSelected() const;

    public slots:
        void settingsChanged(QString key);

    signals:
        void pressed(NewsItemWidget* item);

    private:
        void setColor(QColor bkgColor, QColor textColor);

    private:
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

    private:
        Ui::NewsItemWidget* ui;
        NewsItem item;
        bool selected;
};

#endif // NEWSITEM_H
