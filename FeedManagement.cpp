#include "FeedManagement.h"
#include "ui_FeedManagement.h"

FeedManagement::FeedManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeedManagement)
{
    ui->setupUi(this);
}

FeedManagement::~FeedManagement()
{
    delete ui;
}
