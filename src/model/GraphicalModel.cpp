#include "GraphicalModel.h"

#include "model/GraphicalItem.h"

GraphicalModel::GraphicalModel(GraphicalItem* rootItem, QObject* parent)
    : QAbstractItemModel(parent),
      mRootItem(rootItem)
{
}

GraphicalModel::~GraphicalModel()
{
    delete mRootItem;
}

void GraphicalModel::addGraphicalItem(GraphicalItem* item, GraphicalItem* parent)
{
    if (!parent) {
        parent = mRootItem;
    }
    parent->appendChild(item);
}

QVariant GraphicalModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    GraphicalItem* item = graphicalItemFromIndex(index);
    return item->name();
}

QModelIndex GraphicalModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    GraphicalItem* parentItem = graphicalItemFromIndex(parent);
    GraphicalItem* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex GraphicalModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    GraphicalItem* childItem = graphicalItemFromIndex(child);
    GraphicalItem* parentItem = childItem->parentItem();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int GraphicalModel::rowCount(const QModelIndex& parent) const
{
    GraphicalItem* parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    parentItem = graphicalItemFromIndex(parent);
    return parentItem->childCount();
}

int GraphicalModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

GraphicalItem* GraphicalModel::graphicalItemFromIndex(const QModelIndex& index) const
{
    if (index.isValid()) {
        return static_cast<GraphicalItem*>(index.internalPointer());
    } else {
        return mRootItem;
    }
}

