#ifndef NEWSGROUPWIDGET_H
#define NEWSGROUPWIDGET_H

#include <QWidget>

namespace Ui{
    class NewsGroupWidget;
}

class NewsGroupWidget : public QWidget{
    Q_OBJECT

    public:
        explicit NewsGroupWidget(QWidget* parent = 0);
        ~NewsGroupWidget();

    private:
       void paintEvent(QPaintEvent*);

    private:
        Ui::NewsGroupWidget* ui;
};

#endif // NEWSGROUPWIDGET_H
