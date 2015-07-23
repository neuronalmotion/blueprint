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
    setBrush(QBrush(Qt::blue));
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
    if (change == ItemPositionChange) {
        QPointF delta = value.toPointF() - pos();
        mParentBoundingBox->boundingBoxPointMoved(mTranslationDirection, delta);
    }
    return QGraphicsItem::itemChange(change, value);
}

