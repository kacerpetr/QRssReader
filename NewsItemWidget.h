#ifndef NEWSITEM_H
#define NEWSITEM_H

#include <QWidget>

namespace Ui {
    class NewsItemWidget;
}

class NewsItemWidget : public QWidget{
    Q_OBJECT

    public:
        explicit NewsItemWidget(QWidget *parent = 0);
        ~NewsItemWidget();

    private:
       void paintEvent(QPaintEvent *);

    private:
        Ui::NewsItemWidget *ui;
};

#endif // NEWSITEM_H
