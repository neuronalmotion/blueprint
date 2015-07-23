#include "BoundingBoxPoint.h"

#include <QBrush>
#include <QDebug>

#include "model/BoundingBox.h"

BoundingBoxPoint::BoundingBoxPoint(BoundingBox* parent, TranslationDirection direction)
    : QGraphicsRectItem(parent),
      mParentBoundingBox(parent),
      mTranslationDirection(direction)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setBrush(QBrush(Qt::lightGray));
}

BoundingBoxPoint::~BoundingBoxPoint()
{

}

BoundingBoxPoint::TranslationDirection BoundingBoxPoint::getTranslationDirection() const
{
    return mTranslationDirection;
}

QVariant BoundingBoxPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    QVariant retVal = value;
    if (change == ItemPositionChange) {
        retVal = restrictPosition(value.toPointF());
        QPointF delta = retVal.toPointF() - pos();
        mParentBoundingBox->boundingBoxPointMoved(mTranslationDirection, delta);
    }
    return retVal;
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

