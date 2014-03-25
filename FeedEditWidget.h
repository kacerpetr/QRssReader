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

#ifndef FEEDEDITWIDGET_H
#define FEEDEDITWIDGET_H

#include <QWidget>
#include "RssDataModel.h"

namespace Ui{
    class FeedEditWidget;
}

/**
 * @brief Feed list editor widget
 */
class FeedEditWidget : public QWidget{
    Q_OBJECT

    public:
        explicit FeedEditWidget(QWidget* parent = 0);
        void setModel(RssDataModel* model);
        ~FeedEditWidget();

    private:
        FeedItem makeFeedItem() const;
        void listFeedPresets();
        void fillFeedList();

    signals:
        void closePressed();

    public slots:
        void feedSelected(int row);
        void bkgColorChanged();
        void addNewPressed();
        void saveChangesPressed();
        void removePressed();
        void pasteNamePressed();
        void pasteUrlPressed();
        void pasteDescriptionPressed();
        void emitClosePressed();
        void loadPresetPressed();
        void presetSelected(int currentRow);

    private:
        Ui::FeedEditWidget *ui;
        RssDataModel* model;
        QList<FeedList> lists;
};

#endif // FEEDEDITWIDGET_H
