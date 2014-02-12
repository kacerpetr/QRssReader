#ifndef ACTIONBARWIDGET_H
#define ACTIONBARWIDGET_H

#include <QWidget>
#include <QColor>
#include <QAction>

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
        ~ActionBarWidget();

    private:
        void paintEvent(QPaintEvent*);

    private:
        Ui::ActionBarWidget* ui;
};

#endif // ACTIONBARWIDGET_H
