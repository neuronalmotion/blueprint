#include "BoundingBox.h"

#include "SketchItem.h"

BoundingBox::BoundingBox(SketchItem* parentSketchItem)
    : QGraphicsRectItem(parentSketchItem->getGraphicsItem()),
      mParentSketchItem(parentSketchItem),
    mTopLeftHandle(this, BoundingBoxPoint::TOP_LEFT),
    mTopHandle(this, BoundingBoxPoint::TOP),
    mTopRightHandle(this, BoundingBoxPoint::TOP_RIGHT),
    mRightHandle(this, BoundingBoxPoint::RIGHT),
    mBottomRight(this, BoundingBoxPoint::BOTTOM_RIGHT),
    mBottom(this, BoundingBoxPoint::BOTTOM),
    mBottomLeft(this, BoundingBoxPoint::BOTTOM_LEFT),
    mLeft(this, BoundingBoxPoint::LEFT)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::updateRect()
{
    QRectF boundingRect = mParentSketchItem->getGraphicsItem()->boundingRect();
    qreal marginAdjust = 10;
    boundingRect.adjust(-marginAdjust, -marginAdjust, marginAdjust, marginAdjust);
    setRect(boundingRect);

    qreal x;
    qreal y;
    qreal halfSize = HANDLE_SIZE / 2.0f;

    // position top left handle
    x = boundingRect.x() - halfSize;
    y = boundingRect.y() - halfSize;
    mTopLeftHandle.setRect(x, y, HANDLE_SIZE, HANDLE_SIZE);

    // position top handle
    x = boundingRect.x() + (boundingRect.width() / 2) - halfSize;
    y = boundingRect.y() - halfSize;
    mTopHandle.setRect(x, y, HANDLE_SIZE, HANDLE_SIZE);

    // position top right handle
    x = boundingRect.x() + boundingRect.width() - halfSize;
    y = boundingRect.y() - halfSize;
    mTopRightHandle.setRect(x, y, HANDLE_SIZE, HANDLE_SIZE);
}

void BoundingBox::boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta)
{
    mParentSketchItem->boundBoxPointMoved(direction, delta);
}


