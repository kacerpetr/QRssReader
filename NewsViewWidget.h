#ifndef NEWSVIEWWIDGET_H
#define NEWSVIEWWIDGET_H

#include <QWidget>
#include "RssDataModel.h"

namespace Ui{
    class NewsViewWidget;
}

class NewsViewWidget : public QWidget{
    Q_OBJECT

    public:
        explicit NewsViewWidget(QWidget* parent = 0);
        ~NewsViewWidget();

    public slots:
        void itemPressed(TRssItem* item);
        void settingsChanged(QString tag);

    private:
        Ui::NewsViewWidget* ui;
};

#endif // NEWSVIEWWIDGET_H
