#include "BezierControlPoint.h"

#include <QPen>

#include "SketchItemBezier.h"

BezierControlPoint::BezierControlPoint(SketchItemBezier* item, int index)
    : QGraphicsRectItem(item->getGraphicsItem()),
      mItem(item),
      mIndex(index)
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

QVariant BezierControlPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange) {
         mItem->updateElement(mIndex, value.toPointF());
    }
    return QGraphicsRectItem::itemChange(change, value);
}


