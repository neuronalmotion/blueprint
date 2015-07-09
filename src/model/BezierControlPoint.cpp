#include "BezierControlPoint.h"

#include <QPen>

#include "BezierPath.h"

BezierControlPoint::BezierControlPoint(BezierPath* parent, int index)
    : BezierElement(ElementType::CONTROL_POINT, parent, index),
    QGraphicsRectItem(parent->getGraphicsItem())
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    QPen pen = QPen(Qt::green);
    setPen(pen);

    int rectSize = 10;
    setRect(-rectSize/2, -rectSize/2, rectSize, rectSize);
}

BezierControlPoint::~BezierControlPoint()
{

}

QPointF BezierControlPoint::getPos()
{
    return QGraphicsItem::pos();
}

QVariant BezierControlPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    propagateItemChange(change, value);
    return QGraphicsItem::itemChange(change, value);
}


