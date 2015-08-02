#include "BezierPoint.h"

#include <QPen>

#include "SketchItemBezier.h"

BezierPoint::BezierPoint(SketchItemBezier* parent, const int index)
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
    setVisible(false);
}

BezierPoint::~BezierPoint()
{

}

QPointF BezierPoint::getPos()
{
    return QGraphicsItem::pos();
}

void BezierPoint::setPos(QPointF pos)
{
    QGraphicsItem::setPos(pos);
}

void BezierPoint::moveBy(QPointF delta)
{
    QGraphicsItem::moveBy(delta.x(), delta.y());
}

QVariant BezierPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    propagateItemChange(change, value);
    return QGraphicsItem::itemChange(change, value);
}

