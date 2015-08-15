#include "GraphicalItem.h"

#include <QString>

static int id = 1;

GraphicalItem::GraphicalItem(const GraphicalType& graphicalType, GraphicalItem* parentItem)
    : mGraphicalType(graphicalType),
      mParentItem(parentItem),
      mName(QString("GraphicalItem #%1").arg(id++)),
      mModelIndex(nullptr),
      mIsSelected(false)
{
}

GraphicalItem::~GraphicalItem()
{
    delete mModelIndex;
    qDeleteAll(mChildItems);
}

void GraphicalItem::appendChild(GraphicalItem* child)
{
    mChildItems.append(child);
}

GraphicalItem*GraphicalItem::child(int row)
{
    return mChildItems.at(row);
}

int GraphicalItem::childCount() const
{
    return mChildItems.length();
}

int GraphicalItem::columnCount() const
{
    return 1;
}

int GraphicalItem::row() const
{
    if (mParentItem) {
        return mParentItem->mChildItems.indexOf((const_cast<GraphicalItem*>(this)));
    }
    return 0;
}


