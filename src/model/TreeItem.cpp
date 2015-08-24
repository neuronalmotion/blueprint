#include "TreeItem.h"

#include <QString>
#include <QDebug>

using namespace blueprint;

static int id = 1;

TreeItem::TreeItem(const ItemType& itemType, TreeItem* parentItem)
    : mItemType(itemType),
      mName(QString("GraphicalItem #%1").arg(id++)),
      mParentItem(parentItem),
      mChildItems(),
      mModelIndex(nullptr),
      mIsSelected(false)
{
    static int id = 1;
    mName = QString("GraphicalItem #%1").arg(id++);
}

TreeItem::~TreeItem()
{
    delete mModelIndex;
    qDeleteAll(mChildItems);
}

void TreeItem::appendChild(TreeItem* child)
{
    mChildItems.append(child);
}

TreeItem*TreeItem::child(int row)
{
    return mChildItems.at(row);
}

int TreeItem::indexOf(const TreeItem* child) const
{
    for (int i = 0; i < mChildItems.length(); ++i) {
        if (mChildItems[i] == child) {
            return i;
        }
    }
    return -1;
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

