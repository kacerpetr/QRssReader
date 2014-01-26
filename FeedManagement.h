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

#ifndef FEEDMANAGEMENT_H
#define FEEDMANAGEMENT_H

#include <QDialog>
#include "RssFeedModel.h"

namespace Ui{
    class FeedManagement;
}

/**
 * @brief The FeedManagement dialog class
 * Dialog to edit feed items in list of feeds
 */
class FeedManagement : public QDialog{
    Q_OBJECT

    public:
        explicit FeedManagement(QWidget* parent = 0);
        void setModel(RssFeedModel* model);
        ~FeedManagement();

    private:
        FeedItem makeFeedItem() const;

    public slots:
        void feedSelected(int row);
        void bkgColorChanged();
        void textColorChanged();
        void addNewPressed();
        void saveChangesPressed();
        void removePressed();
        void pasteNamePressed();
        void pasteUrlPressed();
        void pasteDescriptionPressed();

    private:
        Ui::FeedManagement* ui;
        RssFeedModel* model;
};

#endif // FEEDMANAGEMENT_H
