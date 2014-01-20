#ifndef FEEDMANAGEMENT_H
#define FEEDMANAGEMENT_H

#include <QDialog>

namespace Ui {
class FeedManagement;
}

class FeedManagement : public QDialog
{
    Q_OBJECT

public:
    explicit FeedManagement(QWidget *parent = 0);
    ~FeedManagement();

private:
    Ui::FeedManagement *ui;
};

#endif // FEEDMANAGEMENT_H
