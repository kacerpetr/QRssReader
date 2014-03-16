#include "ActionBarWidget.h"
#include "ui_ActionBarWidget.h"
#include <QPainter>
#include <QStyleOption>

/**
 * @brief Class constructor
 */
ActionBarWidget::ActionBarWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ActionBarWidget){
    ui->setupUi(this);
}

/**
 * @brief Class destructor
 */
ActionBarWidget::~ActionBarWidget(){
    delete ui;
}

/**
 * @brief Sets background color of action bar
 */
void ActionBarWidget::setBackgroundColor(QColor color){
    //background color
    QString textClrStr = QString::number(color.red()) + ",";
    textClrStr += QString::number(color.green()) + ",";
    textClrStr += QString::number(color.blue());

    //sets icon stylesheet
    setStyleSheet(
        "#ActionBarWidget{"
        "    background-color: rgb("+textClrStr+");\n"
        "}"
    );
}

/**
 * @brief Adds action to action bar
 */
void ActionBarWidget::addAction(QAction* action, BarActionAlign align){
    //creates toolbutton for given action
    QToolButton* btn = new QToolButton(this);
    btn->setIconSize(QSize(32,32));
    btn->setMaximumSize(50,50);
    btn->setMinimumSize(50,50);
    btn->setDefaultAction(action);
    btn->setStyleSheet(
        "QToolButton{\n"
        "   border: none;\n"
        "   background: none;\n"
        "}\n"
        "QToolButton:hover{\n"
        "   border: none;\n"
        "   background: rgba(255,255,255,10);\n"
        "}\n"
        "QToolButton:pressed{\n"
        "   border: none;\n"
        "   background: rgba(255,255,255,20);\n"
        "}\n"
        "QToolButton:checked{\n"
        "   border: none;\n"
        "   background: rgba(255,255,255,50);\n"
        "}"
    );

    //adds button to action bar
    switch(align){
        case AlignLeft:
            ui->leftlLayout->insertWidget(0, btn);
            break;

        case AlignCenter:
            ui->centerLayout->addWidget(btn);
            break;

        case AlignRight:
            ui->rightLayout->addWidget(btn);
            break;

        default:
            delete btn;
            btn = NULL;
            return;
    }

    //adds button
    btnMap.insert(action, btn);
}

/**
 * @brief Sets action bar content margins
 */
void ActionBarWidget::setMargin(int left, int top, int right, int bottom){
    ui->mainLayout->setContentsMargins(left, top, right, bottom);
}

void ActionBarWidget::hideAction(QAction* action, bool hidden){
    QToolButton* btn = btnMap[action];
    btn->setHidden(hidden);
}

/**
 * @brief Because of using stylesheets
 */
void ActionBarWidget::paintEvent(QPaintEvent*){
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
