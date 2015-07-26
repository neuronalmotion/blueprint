#include "BoundingBoxPoint.h"

#include <QBrush>
#include <QDebug>

#include "model/BoundingBox.h"

BoundingBoxPoint::BoundingBoxPoint(BoundingBox* parent, TranslationDirection direction)
    : QGraphicsRectItem(parent),
      mParentBoundingBox(parent),
      mTranslationDirection(direction)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setBrush(QBrush(Qt::lightGray));
}

BoundingBoxPoint::~BoundingBoxPoint()
{

}

BoundingBoxPoint::TranslationDirection BoundingBoxPoint::getTranslationDirection() const
{
    return mTranslationDirection;
}

void BoundingBoxPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton == Qt::LeftButton)
    {
        QPointF delta = restrictPosition(event->pos() - event->lastPos());
        mParentBoundingBox->boundingBoxPointMoved(mTranslationDirection, delta);
    }
}

QPointF BoundingBoxPoint::restrictPosition(const QPointF& newPosition)
{
    QPointF retVal = pos();

    switch (mTranslationDirection) {
    case TOP:
    case BOTTOM:
        retVal.setY(newPosition.y());
        break;

    case RIGHT:
    case LEFT:
        retVal.setX(newPosition.x());
        break;

    case TOP_LEFT:
    case TOP_RIGHT:
    case BOTTOM_RIGHT:
    case BOTTOM_LEFT:
        retVal = newPosition;
        break;
    default:
        break;
    }
    return retVal;
}

