#include "GraphicalItem.h"

#include <QString>

GraphicalItem::GraphicalItem(GraphicalItem* parentItem)
    : mParentItem(parentItem)
{
    static int id = 1;
    mName = QString("GraphicalItem #%1").arg(id++);
}

GraphicalItem::~GraphicalItem()
{
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

GraphicalItem*GraphicalItem::parentItem()
{
    return mParentItem;
}

QString GraphicalItem::name() const
{
    return mName;
}

void GraphicalItem::setName(const QString& name)
{
    mName = name;
}

void GraphicalItem::setParent(GraphicalItem* parentItem)
{
    mParentItem = parentItem;
}

