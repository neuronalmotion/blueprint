#include "ShapeModel.h"

#include "Shape.h"

using namespace blueprint;

ShapeModel* ShapeModel::sInstance = nullptr;

ShapeModel::ShapeModel()
    : QAbstractItemModel(),
    mRootItem(nullptr)
{
}

ShapeModel::~ShapeModel()
{
}

ShapeModel* ShapeModel::instance()
{
    if (!sInstance) {
        sInstance = new ShapeModel();
    }
    return sInstance;
}

void ShapeModel::addItem(Shape* item, Shape* parent)
{
    if (!parent) {
        parent = mRootItem;
    }
    int childRow = parent->childCount();
    QModelIndex parentIndex = parent->modelIndex() ?
                (QModelIndex)(*parent->modelIndex())
                : QModelIndex();
    beginInsertRows(parentIndex, childRow, childRow);
    item->setParentShape(parent);
    parent->appendChild(item);
    QModelIndex childIndex = index(childRow, 0, parentIndex);
    item->setModelIndex(childIndex);
    endInsertRows();
}

QVariant ShapeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    Shape* item = itemFromIndex(index);
    return item->name();
}

QModelIndex ShapeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    Shape* parentItem = itemFromIndex(parent);
    Shape* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex ShapeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    Shape* childItem = itemFromIndex(child);
    Shape* parentItem = childItem->parentShape();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ShapeModel::rowCount(const QModelIndex& parent) const
{
    Shape* parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    parentItem = itemFromIndex(parent);
    return parentItem->childCount();
}

int ShapeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

bool ShapeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    Shape* item = itemFromIndex(index);
    item->setName(value.toString());
    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags ShapeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void ShapeModel::setRootItem(Shape* rootItem)
{
    mRootItem = rootItem;
    mRootItem->setModelIndex(index(0, 0));
}

Shape* ShapeModel::itemFromIndex(const QModelIndex& index) const
{
    if (index.isValid()) {
        return static_cast<Shape*>(index.internalPointer());
    } else {
        return mRootItem;
    }
}

Shape* ShapeModel::itemFromParentIndex(const QModelIndex& parentIndex, int row) const
{
    Shape* parentItem = itemFromIndex(parentIndex);
    return parentItem->child(row);
}
