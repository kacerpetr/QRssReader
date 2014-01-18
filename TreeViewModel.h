#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class TreeViewModel : public QAbstractItemModel{
    Q_OBJECT

    public:
        explicit TreeViewModel(QObject* parent = 0);
        QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex& child) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

#endif
