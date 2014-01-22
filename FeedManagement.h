#ifndef FEEDMANAGEMENT_H
#define FEEDMANAGEMENT_H

#include <QDialog>
#include "RssFeedModel.h"

namespace Ui {
    class FeedManagement;
}

class FeedManagement : public QDialog{
    Q_OBJECT

    public:
        explicit FeedManagement(QWidget* parent = 0);
        void setModel(RssFeedModel* model);
        FeedItem makeFeedItem() const;
        ~FeedManagement();

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
