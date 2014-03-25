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

#ifndef ACTIONBARWIDGET_H
#define ACTIONBARWIDGET_H

#include <QWidget>
#include <QColor>
#include <QAction>
#include <QMap>
#include <QToolButton>

/**
 * @brief Alignment of item placed in ActionBar
 */
enum BarActionAlign{
    AlignLeft,
    AlignCenter,
    AlignRight
};

namespace Ui{
    class ActionBarWidget;
}

/**
 * @brief The ActionBarWidget class
 */
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
