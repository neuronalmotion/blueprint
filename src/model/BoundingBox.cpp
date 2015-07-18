#include "BoundingBox.h"

#include "SketchItem.h"

BoundingBox::BoundingBox(SketchItem* parentSketchItem)
    : QGraphicsRectItem(parentSketchItem->getGraphicsItem()),
      mParentSketchItem(parentSketchItem)
{
    mTopHandle.setParentItem(this);
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::updateRect()
{
    QRectF boundingRect = mParentSketchItem->getGraphicsItem()->boundingRect();
    setRect(boundingRect);

    qreal x;
    qreal y;
    qreal halfSize = HANDLE_SIZE / 2.0f;

    // position top handle
    x = boundingRect.x() + (boundingRect.width() / 2) - halfSize;
    y = boundingRect.y() - halfSize;
    mTopHandle.setRect(x, y, HANDLE_SIZE, HANDLE_SIZE);
}

