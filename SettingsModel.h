#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QObject>
#include <QString>
#include <QMap>

#define SETTINGS_FILE "settings.xml"

class SettingsModel : public QObject{
    Q_OBJECT

    public:
        static SettingsModel& get();
        void loadSettings();
        void saveSettings();
        void setValue(const QString& tag, const QString& value);
        void setValue(const QString& tag, const int value);
        void setValue(const QString& tag, const double value);
        QString getText(const QString& tag) const;
        int getInt(const QString& tag) const;
        double getDouble(const QString& tag) const;

     signals:
        void dataChanged(QString);

     private:
        QMap<QString,QString> data;

     private:
        SettingsModel(QObject* parent = 0):QObject(parent){}
        SettingsModel(SettingsModel const&);
        void operator=(SettingsModel const&);
};

#endif // SETTINGSMODEL_H
