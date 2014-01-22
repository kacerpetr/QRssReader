#ifndef STORAGEACCESS_H
#define STORAGEACCESS_H

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define APP_FOLDER (".qrssreader")

class StorageAccess{
    public:
        static StorageAccess& get();
        QString appDir();
        QString absPath(QString filename);
        QXmlStreamReader* openXmlReader(QString filename);
        void closeXmlReader(QXmlStreamReader** reader);
        QXmlStreamWriter* openXmlWriter(QString filename);
        void closeXmlWriter(QXmlStreamWriter** writer);

    private:
        StorageAccess(){}
        StorageAccess(StorageAccess const&);
        void operator=(StorageAccess const&);
};

#endif // STORAGEACCESS_H
