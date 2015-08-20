#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QPointF>

class TreeItem;

class TreelModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreelModel(TreeItem* rootItem = 0, QObject* parent = 0);
    ~TreelModel();

    void addItem(TreeItem* item, TreeItem* parent = 0, const QModelIndex& parentIndex = QModelIndex());
    TreeItem* itemFromIndex(const QModelIndex& index) const;

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:

private:
    TreeItem* mRootItem;
};

#endif // TREEMODEL_H
