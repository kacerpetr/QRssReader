#include "HelpDialog.h"
#include "ui_HelpDialog.h"
#include <QPainter>

HelpDialog::HelpDialog(QWidget* parent) : QDialog(parent), ui(new Ui::HelpDialog){
    ui->setupUi(this);
}

void HelpDialog::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HelpDialog::~HelpDialog(){
    delete ui;
}
