#include "BezierPoint.h"

#include <QPen>

#include "ShapeBezier.h"

using namespace blueprint;

BezierPoint::BezierPoint(ShapeBezier* parent, const int index)
    : BezierElement(ElementType::POINT, parent, index),
    QGraphicsEllipseItem(parent)
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

QPointF BezierPoint::pos() const
{
    return QGraphicsItem::pos();
}

void BezierPoint::setPos(const QPointF& pos)
{
    QGraphicsItem::setPos(pos);
}

void BezierPoint::moveBy(const QPointF& delta)
{
    QGraphicsItem::moveBy(delta.x(), delta.y());
}

void BezierPoint::setVisible(bool isVisible)
{
    QGraphicsItem::setVisible(isVisible);
}

QVariant BezierPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    propagateItemChange(change, value);
    return QGraphicsItem::itemChange(change, value);
}

