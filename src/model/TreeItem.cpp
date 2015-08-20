#include "TreeItem.h"

#include <QString>

static int id = 1;

TreeItem::TreeItem(const ItemType& itemType, TreeItem* parentItem)
    : mItemType(itemType),
      mParentItem(parentItem),
      mName(QString("GraphicalItem #%1").arg(id++)),
      mModelIndex(nullptr),
      mIsSelected(false)
{
    static int id = 1;
    mName = QString("GraphicalItem #%1").arg(id++);

    if (parentItem != nullptr) {
        QGraphicsItem* parentGraphicsItem = parentItem->getGraphicsItem();
        QGraphicsItem* graphicsItem = getGraphicsItem();
        if (parentGraphicsItem != nullptr && graphicsItem != nullptr) {
            graphicsItem->setParentItem(parentGraphicsItem);
        }
    }
}

TreeItem::~TreeItem()
{
    delete mModelIndex;
    qDeleteAll(mChildItems);
}

void TreeItem::appendChild(TreeItem* child)
{
    mChildItems.append(child);

    QGraphicsItem* childGraphicsItem = child->getGraphicsItem();
    QGraphicsItem* graphicsItem = getGraphicsItem();
    if (childGraphicsItem != nullptr && graphicsItem != nullptr) {
        childGraphicsItem->setParentItem(graphicsItem);
    }
}

TreeItem*TreeItem::child(int row)
{
    return mChildItems.at(row);
}

int TreeItem::childCount() const
{
    return mChildItems.length();
}

int TreeItem::columnCount() const
{
    return 1;
}

int TreeItem::row() const
{
    if (mParentItem) {
        return mParentItem->mChildItems.indexOf((const_cast<TreeItem*>(this)));
    }
    return 0;
}

