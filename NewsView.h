#ifndef NEWSVIEW_H
#define NEWSVIEW_H

#include <QScrollArea>

class NewsView : public QScrollArea{
    Q_OBJECT

    public:
        explicit NewsView(QWidget *parent = 0);

    signals:

    public slots:

};

#endif // NEWSVIEW_H
