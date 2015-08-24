#include "TreeModel.h"

#include "TreeItem.h"

using namespace blueprint;

TreeModel* TreeModel::sInstance = nullptr;

TreeModel::TreeModel()
    : QAbstractItemModel(),
    mRootItem(nullptr)
{
}

TreeModel::~TreeModel()
{
    delete mRootItem;
}

TreeModel* TreeModel::instance()
{
    if (!sInstance) {
        sInstance = new TreeModel();
    }
    return sInstance;
}

void TreeModel::addItem(TreeItem* item, TreeItem* parent)
{
    if (!parent) {
        parent = mRootItem;
    }
    int childRow = parent->childCount();
    QModelIndex parentIndex = parent->modelIndex() ?
                (QModelIndex)(*parent->modelIndex())
                : QModelIndex();
    beginInsertRows(parentIndex, childRow, childRow);
    item->setParentTreeItem(parent);
    parent->appendChild(item);
    QModelIndex childIndex = index(childRow, 0, parentIndex);
    item->setModelIndex(childIndex);
    endInsertRows();
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
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

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
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

QModelIndex TreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    TreeItem* childItem = itemFromIndex(child);
    TreeItem* parentItem = childItem->parentTreeItem();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    parentItem = itemFromIndex(parent);
    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    TreeItem* item = itemFromIndex(index);
    item->setName(value.toString());
    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void TreeModel::setRootItem(TreeItem* rootItem)
{
    mRootItem = rootItem;
    mRootItem->setModelIndex(index(0, 0));
}

TreeItem* TreeModel::itemFromIndex(const QModelIndex& index) const
{
    if (index.isValid()) {
        return static_cast<TreeItem*>(index.internalPointer());
    } else {
        return mRootItem;
    }
}

TreeItem* TreeModel::itemFromParentIndex(const QModelIndex& parentIndex, int row) const
{
    TreeItem* parentItem = itemFromIndex(parentIndex);
    return parentItem->child(row);
}
