#include "BezierPoint.h"

#include <QPen>

#include "SketchItemBezier.h"

BezierPoint::BezierPoint(SketchItemBezier* item, const int index)
    : BezierElement(item, index),
    QGraphicsEllipseItem(item->getGraphicsItem())
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

QVariant BezierPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    propagateItemChange(change, value);
    return QGraphicsItem::itemChange(change, value);
}

