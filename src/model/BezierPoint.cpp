#include "BezierPoint.h"

#include <QPen>

#include "BezierPath.h"

BezierPoint::BezierPoint(BezierPath* parent, const int index)
    : BezierElement(ElementType::POINT, parent, index),
    QGraphicsEllipseItem(parent->getGraphicsItem())
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setBrush(QBrush(Qt::red));
    int rectSize = 10;
    setRect(-rectSize/2, -rectSize/2, rectSize, rectSize);
}

BezierPoint::~BezierPoint()
{

}

QPointF BezierPoint::getPos()
{
    return QGraphicsItem::pos();
}

QVariant BezierPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    propagateItemChange(change, value);
    return QGraphicsItem::itemChange(change, value);
}

