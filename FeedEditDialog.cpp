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

#include "FeedEditDialog.h"
#include "ui_FeedEditDialog.h"
#include "FeedEditWidget.h"

/**
 * @brief Class constructor
 * @param parent
 */
FeedEditDialog::FeedEditDialog(QWidget* parent) : QDialog(parent), ui(new Ui::FeedEditDialog), models(NULL){
    ui->setupUi(this);

    //creates tabs ant their widgets
    for(int i = 0; i < TAB_COUNT; i++){
        FeedEditWidget* few = new FeedEditWidget(this);
        ui->tabWidget->addTab(few, "Tab " + QString::number(i+1));
        ui->tabWidget->setTabIcon(i, QIcon(":/images/list_tab" + QString::number(i+1)));
        connect(few, SIGNAL(closePressed()), this, SLOT(close()));
    }
}

/**
 * @brief Class destructor
 */
FeedEditDialog::~FeedEditDialog(){
    delete ui;
}

/**
 * @brief Sets RssFeedModel pointer
 * @param models
 * @see RssFeedModel
 */
void FeedEditDialog::setModels(QList<RssDataModel*>* models){
    this->models = models;

    for(int i = 0; i < models->length(); i++){
        FeedEditWidget* few = (FeedEditWidget*)ui->tabWidget->widget(i);
        few->setModel(models->at(i));
    }
}
