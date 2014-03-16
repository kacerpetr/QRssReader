#ifndef ACTIONBARWIDGET_H
#define ACTIONBARWIDGET_H

#include <QWidget>
#include <QColor>
#include <QAction>
#include <QMap>
#include <QToolButton>

enum BarActionAlign{
    AlignLeft,
    AlignCenter,
    AlignRight
};

namespace Ui {
    class ActionBarWidget;
}

class ActionBarWidget : public QWidget{
    Q_OBJECT

    public:
        explicit ActionBarWidget(QWidget* parent = 0);
        void setBackgroundColor(QColor color);
        void addAction(QAction* action, BarActionAlign align);
        void setMargin(int left, int top, int right, int bottom);
        void hideAction(QAction* action, bool hidden);
        ~ActionBarWidget();

    private:
        void paintEvent(QPaintEvent*);

    private:
        Ui::ActionBarWidget* ui;
        QMap<QAction*,QToolButton*> btnMap;
};

#endif // ACTIONBARWIDGET_H
