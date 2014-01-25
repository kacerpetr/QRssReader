#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <RssFeedModel.h>
#include <RssDataModel.h>

#define FEEDS_PREFIX ":/feedlist/feedlist"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog{
    Q_OBJECT

    public:
        explicit SettingsDialog(QWidget* parent = 0);
        void setRssFeedModel(RssFeedModel* model);
        void setRssDataModel(RssDataModel* model);
        ~SettingsDialog();

    public slots:
        void clearCachePressed();
        void savePressed();
        void loadPresetPressed();

    private:
        Ui::SettingsDialog* ui;
        RssFeedModel* feedModel;
        RssDataModel* dataModel;
};

#endif // SETTINGSDIALOG_H
