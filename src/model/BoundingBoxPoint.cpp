#include "BoundingBoxPoint.h"

#include <QBrush>
#include <QDebug>

#include "model/BoundingBox.h"
#include "model/Shape.h"

using namespace blueprint;

BoundingBoxPoint::BoundingBoxPoint(BoundingBox* parent, TranslationDirection direction)
    : QGraphicsRectItem(parent),
      mParentBoundingBox(parent),
      mTranslationDirection(direction)
{
    //setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setBrush(QBrush(Qt::lightGray));

    qreal halfSize = HANDLE_SIZE / 2.0f;
    setRect(-halfSize, -halfSize, HANDLE_SIZE, HANDLE_SIZE);

    setData(Shape::ShapeType::BOUNDING_BOX_POINT, qVariantFromValue(static_cast<void *>(this)));
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
    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
    {
        QPointF delta = restrictPosition(event->pos() - event->lastPos());
        //QPointF delta = restrictPosition(event->pos() - pos());
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

