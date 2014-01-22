#include "FeedManagement.h"
#include "ui_FeedManagement.h"
#include <QMessageBox>
#include <QClipboard>

FeedManagement::FeedManagement(QWidget* parent) : QDialog(parent), ui(new Ui::FeedManagement), model(NULL){
    ui->setupUi(this);

    //background color preview
    connect(ui->bkgRedBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));
    connect(ui->bkgGreenBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));
    connect(ui->bkgBlueBox, SIGNAL(valueChanged(int)), this, SLOT(bkgColorChanged()));

    //text color preview
    connect(ui->textRedBox, SIGNAL(valueChanged(int)), this, SLOT(textColorChanged()));
    connect(ui->textGreenBox, SIGNAL(valueChanged(int)), this, SLOT(textColorChanged()));
    connect(ui->textBlueBox, SIGNAL(valueChanged(int)), this, SLOT(textColorChanged()));

    //feed list selection
    connect(ui->feedList, SIGNAL(currentRowChanged(int)), this, SLOT(feedSelected(int)));

    //main control buttons
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(saveChangesPressed()));
    connect(ui->addButton, SIGNAL(pressed()), this, SLOT(addNewPressed()));
    connect(ui->removeButton, SIGNAL(pressed()), this, SLOT(removePressed()));

    //paste buttons
    connect(ui->pasteNameButton, SIGNAL(pressed()), this, SLOT(pasteNamePressed()));
    connect(ui->urlNameButton, SIGNAL(pressed()), this, SLOT(pasteUrlPressed()));
    connect(ui->descriptionNameButton, SIGNAL(pressed()), this, SLOT(pasteDescriptionPressed()));
}

void FeedManagement::setModel(RssFeedModel* model){
    //saves model pointer
    this->model = model;

    //list of feeds
    const QList<FeedItem>& items = model->feedList();

    //fills list widget
    for(int i = 0; i < items.length(); i++){
        ui->feedList->addItem(items[i].name + " - " + items[i].description);
    }

    //selects first item
    ui->feedList->setCurrentRow(0);
}

FeedItem FeedManagement::makeFeedItem() const{
    FeedItem item;

    //main properties
    item.name = ui->nameEdit->text();
    item.url = ui->urlEdit->text();
    item.description = ui->descriptionEdit->text();
    item.enabled = ui->enableBox->isChecked();

    //background color in news list
    item.bkgColor = QColor(
        ui->bkgRedBox->value(),
        ui->bkgGreenBox->value(),
        ui->bkgBlueBox->value()
    );

    //text color in news list
    item.textColor = QColor(
        ui->textRedBox->value(),
        ui->textGreenBox->value(),
        ui->textBlueBox->value()
    );

    return item;
}

void FeedManagement::feedSelected(int row){
    //invalid row check
    if(row < 0) return;

    //list of feeds
    const QList<FeedItem>& items = model->feedList();

    //sets base properties of feed
    ui->nameEdit->setText(items[row].name);
    ui->urlEdit->setText(items[row].url);
    ui->descriptionEdit->setText(items[row].description);
    ui->enableBox->setChecked(items[row].enabled);

    //background color in news list
    ui->bkgRedBox->setValue(items[row].bkgColor.red());
    ui->bkgGreenBox->setValue(items[row].bkgColor.green());
    ui->bkgBlueBox->setValue(items[row].bkgColor.blue());

    //text color in news list
    ui->textRedBox->setValue(items[row].textColor.red());
    ui->textGreenBox->setValue(items[row].textColor.green());
    ui->textBlueBox->setValue(items[row].textColor.blue());

    //renew color previews
    bkgColorChanged();
    textColorChanged();
}

void FeedManagement::bkgColorChanged(){
    QString color = QString::number(ui->bkgRedBox->value()) + ",";
    color += QString::number(ui->bkgGreenBox->value()) + ",";
    color += QString::number(ui->bkgBlueBox->value());
    ui->bkgColorPreview->setStyleSheet("background-color: rgb(" + color + ");");
}

void FeedManagement::textColorChanged(){
    QString color = QString::number(ui->textRedBox->value()) + ",";
    color += QString::number(ui->textGreenBox->value()) + ",";
    color += QString::number(ui->textBlueBox->value());
    ui->textColorPreview->setStyleSheet("background-color: rgb(" + color + ");");
}

void FeedManagement::addNewPressed(){
    FeedItem item = makeFeedItem();

    //adds feed
    if(!model->addFeed(item)){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error adding new feed");
        msgBox.setText("Feed with given name or url already exists");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    //adds item to feed list
    ui->feedList->addItem(item.name + " - " + item.description);

    //saves feed list
    model->saveFeedList();
}

void FeedManagement::saveChangesPressed(){
    //index of selected item
    int index = ui->feedList->currentRow();
    if(index < 0) return;

    //modified feed
    FeedItem feed = makeFeedItem();

    //modifies feed
    if(!model->modifyFeed(index, feed)){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error saving feed changes");
        msgBox.setText("Name/url was changed to name/url of another existing item");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    //modifies item in feed list
     QListWidgetItem* item = ui->feedList->currentItem();
     item->setText(feed.name + " - " + feed.description);

     //saves feed list
     model->saveFeedList();
}

void FeedManagement::removePressed(){
    //index of selected item
    int index = ui->feedList->currentRow();
    if(index < 0) return;

    //removes item from feed list
    QListWidgetItem* item = ui->feedList->takeItem(index);
    delete item;
    item = NULL;

    //removes feed from models
    model->removeFeed(index);

    //saves feed list
    model->saveFeedList();
}

void FeedManagement::pasteNamePressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->nameEdit->setText(clipboard->text());
}

void FeedManagement::pasteUrlPressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->urlEdit->setText(clipboard->text());
}

void FeedManagement::pasteDescriptionPressed(){
    QClipboard *clipboard = QApplication::clipboard();
    ui->descriptionEdit->setText(clipboard->text());
}

FeedManagement::~FeedManagement(){
    delete ui;
}
