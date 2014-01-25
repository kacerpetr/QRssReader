#ifndef STORAGEACCESS_H
#define STORAGEACCESS_H

#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringList>

#define APP_FOLDER (".qrssreader")

class StorageAccess{
    public:
        static StorageAccess& get();
        QString appDir() const;
        QString absPath(QString filename) const;
        QXmlStreamReader* openXmlReader(QString filename) const;
        void closeXmlReader(QXmlStreamReader** reader) const;
        QXmlStreamWriter* openXmlWriter(QString filename) const;
        void closeXmlWriter(QXmlStreamWriter** writer) const;
        bool writeString(const QString& str, const QString& relativePath) const;
        bool readString(QString& result, const QString& relativePath) const;
        bool mkDir(const QString& relativePath) const;
        bool exists(const QString& relativePath) const;
        bool clearDir(const QString& relativePath) const;
        bool rmFile(const QString& relativePath) const;

     private:
        StorageAccess(){}
        StorageAccess(StorageAccess const&);
        void operator=(StorageAccess const&);
};

#endif // STORAGEACCESS_H
