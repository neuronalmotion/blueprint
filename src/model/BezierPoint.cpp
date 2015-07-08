#include "BezierPoint.h"

#include <QPen>

#include "SketchItemBezier.h"

BezierPoint::BezierPoint(SketchItemBezier* item, const int index)
    : QGraphicsEllipseItem(item->getGraphicsItem()),
    mItem(item),
    mIndex(index)
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
    if (change == ItemPositionChange) {
         mItem->updateElement(mIndex, value.toPointF());
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

