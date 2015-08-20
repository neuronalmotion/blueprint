#include "TreeModel.h"

#include "model/TreeItem.h"

TreelModel::TreelModel(TreeItem* rootItem, QObject* parent)
    : QAbstractItemModel(parent),
      mRootItem(rootItem)
{
}

TreelModel::~TreelModel()
{
    delete mRootItem;
}

void TreelModel::addItem(TreeItem* item, TreeItem* parent, const QModelIndex& parentIndex)
{
    if (!parent) {
        parent = mRootItem;
    }
    int childRow = parent->childCount();
    beginInsertRows(parentIndex, childRow, childRow);
    item->setParent(parent);
    parent->appendChild(item);
    QModelIndex childIndex = index(childRow, 0, parentIndex);
    item->setModelIndex(childIndex);
    endInsertRows();
}

QVariant TreelModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    TreeItem* item = itemFromIndex(index);
    return item->name();
}

QModelIndex TreelModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem* parentItem = itemFromIndex(parent);
    TreeItem* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex TreelModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    TreeItem* childItem = itemFromIndex(child);
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreelModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    parentItem = itemFromIndex(parent);
    return parentItem->childCount();
}

int TreelModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

bool TreelModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    TreeItem* item = itemFromIndex(index);
    item->setName(value.toString());
    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags TreelModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem* TreelModel::itemFromIndex(const QModelIndex& index) const
{
    if (index.isValid()) {
        return static_cast<TreeItem*>(index.internalPointer());
    } else {
        return mRootItem;
    }
}
