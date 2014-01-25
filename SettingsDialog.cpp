#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "SettingsDialog.h"
#include "SettingsModel.h"
#include "StorageAccess.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SettingsDialog){
    ui->setupUi(this);

    //fills form with data
    ui->listWidthBox->setValue(SettingsModel::get().getInt("list_width"));
    ui->listTitleSizeBox->setValue(SettingsModel::get().getInt("list_title_font_size"));
    ui->listTimeFontSize->setValue(SettingsModel::get().getInt("list_time_font_size"));
    ui->viewTitleSize->setValue(SettingsModel::get().getInt("view_title_font_size"));
    ui->viewTextSize->setValue(SettingsModel::get().getInt("view_text_font_size"));
    ui->viewFooterSize->setValue(SettingsModel::get().getInt("view_footer_font_size"));

    //connects
    connect(ui->saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(ui->loadButton, SIGNAL(pressed()), this, SLOT(loadPresetPressed()));
    connect(ui->clearCacheButton, SIGNAL(pressed()), this, SLOT(clearCachePressed()));
}

SettingsDialog::~SettingsDialog(){
    delete ui;
}

void SettingsDialog::clearCachePressed(){

}

void SettingsDialog::savePressed(){
    SettingsModel& sm = SettingsModel::get();

    if(ui->listWidthBox->value() != sm.getInt("list_width"))
        sm.setValue("list_width", ui->listWidthBox->value());

    if(ui->listTitleSizeBox->value() != sm.getInt("list_title_font_size"))
        sm.setValue("list_title_font_size", ui->listTitleSizeBox->value());

    if(ui->listTimeFontSize->value() != sm.getInt("list_time_font_size"))
        sm.setValue("list_time_font_size", ui->listTimeFontSize->value());

    if(ui->viewTitleSize->value() != sm.getInt("view_title_font_size"))
        sm.setValue("view_title_font_size", ui->viewTitleSize->value());

    if(ui->viewTextSize->value() != sm.getInt("view_text_font_size"))
        sm.setValue("view_text_font_size", ui->viewTextSize->value());

    if(ui->viewFooterSize->value() != sm.getInt("view_footer_font_size"))
        sm.setValue("view_footer_font_size", ui->viewFooterSize->value());

    //saves changed data
    sm.saveSettings();

    //closes dialog window
    close();
}

void SettingsDialog::loadPresetPressed(){

}
