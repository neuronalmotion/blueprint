#include "BoundingBox.h"

#include <QDebug>

#include "SketchItem.h"

BoundingBox::BoundingBox(SketchItem* parentSketchItem)
    : QGraphicsRectItem(parentSketchItem->getGraphicsItem()),
      mParentSketchItem(parentSketchItem),
    mBoundingBoxEvent({QPointF(), QPointF(), QPointF()})
{
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::TOP_LEFT, new BoundingBoxPoint(this, BoundingBoxPoint::TOP_LEFT));
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::TOP, new BoundingBoxPoint(this, BoundingBoxPoint::TOP));
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::TOP_RIGHT, new BoundingBoxPoint(this, BoundingBoxPoint::TOP_RIGHT));
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::RIGHT, new BoundingBoxPoint(this, BoundingBoxPoint::RIGHT));
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::BOTTOM_RIGHT, new BoundingBoxPoint(this, BoundingBoxPoint::BOTTOM_RIGHT));
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::BOTTOM, new BoundingBoxPoint(this, BoundingBoxPoint::BOTTOM));
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::BOTTOM_LEFT, new BoundingBoxPoint(this, BoundingBoxPoint::BOTTOM_LEFT));
    mHashBoundingBoxPoints.insert(BoundingBoxPoint::LEFT, new BoundingBoxPoint(this, BoundingBoxPoint::LEFT));

    qDebug() << "mHashBoundingBoxPoints : BOTTOM_RIGHT : " << mHashBoundingBoxPoints[BoundingBoxPoint::BOTTOM_RIGHT]->pos();
    qDebug() << "mHashBoundingBoxPoints : TOP_LEFT : " << mHashBoundingBoxPoints[BoundingBoxPoint::TOP_LEFT]->pos();
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::updateRect(BoundingBoxPoint::TranslationDirection ignoredDirection)
{
    QRectF boundingRect = mParentSketchItem->getGraphicsItem()->boundingRect();
    qreal marginAdjust = 10;
    //boundingRect.adjust(-marginAdjust, -marginAdjust, marginAdjust, marginAdjust);
    setRect(boundingRect);

    qreal x;
    qreal y;

    // position top left handle
    if (ignoredDirection != BoundingBoxPoint::TOP_LEFT) {
        x = boundingRect.x();
        y = boundingRect.y();
        mHashBoundingBoxPoints[BoundingBoxPoint::TOP_LEFT]->setPos(x, y);
    }

    // position top handle
    if (ignoredDirection != BoundingBoxPoint::TOP) {
        x = boundingRect.x() + (boundingRect.width() / 2);
        y = boundingRect.y();
        mHashBoundingBoxPoints[BoundingBoxPoint::TOP]->setPos(x, y);
    }

    // position top right handle
    if (ignoredDirection != BoundingBoxPoint::TOP_RIGHT) {
        x = boundingRect.x() + boundingRect.width();
        y = boundingRect.y();
        mHashBoundingBoxPoints[BoundingBoxPoint::TOP_RIGHT]->setPos(x, y);
    }

    // position right handle
    if (ignoredDirection != BoundingBoxPoint::RIGHT) {
        x = boundingRect.x() + boundingRect.width();
        y = boundingRect.y() + (boundingRect.height() / 2);
        mHashBoundingBoxPoints[BoundingBoxPoint::RIGHT]->setPos(x, y);
    }

    // position bottom right handle
    if (ignoredDirection != BoundingBoxPoint::BOTTOM_RIGHT) {
        x = boundingRect.x() + boundingRect.width();
        y = boundingRect.y() + boundingRect.height();
        mHashBoundingBoxPoints[BoundingBoxPoint::BOTTOM_RIGHT]->setPos(x, y);
    }

    // position bottom handle
    if (ignoredDirection != BoundingBoxPoint::BOTTOM) {
        x = boundingRect.x() + (boundingRect.width() / 2);
        y = boundingRect.y() + boundingRect.height();
        mHashBoundingBoxPoints[BoundingBoxPoint::BOTTOM]->setPos(x, y);
    }

    // position bottom left handle
    if (ignoredDirection != BoundingBoxPoint::BOTTOM_LEFT) {
        x = boundingRect.x();
        y = boundingRect.y() + boundingRect.height();
        mHashBoundingBoxPoints[BoundingBoxPoint::BOTTOM_LEFT]->setPos(x, y);
    }

    // position left handle
    if (ignoredDirection != BoundingBoxPoint::LEFT) {
        x = boundingRect.x();
        y = boundingRect.y() + (boundingRect.height() / 2);
        mHashBoundingBoxPoints[BoundingBoxPoint::LEFT]->setPos(x, y);
    }
}

void BoundingBox::boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta)
{

    QPointF handlePoint;
    QPointF origin;

    qreal halfWidth = boundingRect().width() / 2.0f;
    qreal halfHeight = boundingRect().height() / 2.0f;

    switch (direction) {

        case BoundingBoxPoint::TOP_LEFT:
            origin = boundingRect().bottomRight();
            handlePoint = boundingRect().topLeft();
            break;

        case BoundingBoxPoint::TOP:
            origin = boundingRect().bottomLeft();
            origin.setX(origin.x() + halfWidth);

            handlePoint = boundingRect().topLeft();
            handlePoint.setX(handlePoint.x() + halfWidth);
            break;

        case BoundingBoxPoint::TOP_RIGHT:
            origin = boundingRect().bottomLeft();
            handlePoint = boundingRect().topRight();
            break;

        case BoundingBoxPoint::RIGHT:
            origin = boundingRect().topLeft();
            origin.setY(origin.y() + halfHeight);

            handlePoint = boundingRect().topRight();
            handlePoint.setY( handlePoint.y() + halfHeight);
            break;

        case BoundingBoxPoint::BOTTOM_RIGHT:
            origin = boundingRect().topLeft();
            handlePoint = boundingRect().bottomRight();
            break;

        case BoundingBoxPoint::BOTTOM:
            origin = boundingRect().topLeft();
            origin.setX(origin.x() + halfWidth);

            handlePoint = boundingRect().bottomLeft();
            handlePoint.setX(handlePoint.x() + halfWidth);
            break;

        case BoundingBoxPoint::BOTTOM_LEFT:
            origin = boundingRect().topRight();
            handlePoint = boundingRect().bottomLeft();
            break;

        case BoundingBoxPoint::LEFT:
            origin = boundingRect().topRight();
            origin.setY(origin.y() + halfHeight);

            handlePoint = boundingRect().topLeft();
            handlePoint.setY( handlePoint.y() + halfHeight);
            break;


        default:
            qWarning() << "NYI !!";
            break;
    }

    // Temp scale vars
    QPointF scaleAim = handlePoint + delta - origin;
    QPointF scaleBase = handlePoint - origin;

    // Clamp it!
    if (scaleBase.x() == 0){
        scaleBase.setX(scaleAim.x());
    }

    if (scaleBase.y() == 0){
        scaleBase.setY(scaleAim.y());
    }

    // Set the final values
    mBoundingBoxEvent.origin = origin;
    mBoundingBoxEvent.delta = delta;
    mBoundingBoxEvent.scale.setX( scaleAim.x() / scaleBase.x());
    mBoundingBoxEvent.scale.setY( scaleAim.y() / scaleBase.y());

    mParentSketchItem->boundingBoxEvent(mBoundingBoxEvent);
}


