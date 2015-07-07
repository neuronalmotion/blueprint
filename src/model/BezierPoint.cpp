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
    setPen(QPen(Qt::red));
    int rectSize = 10;
    setRect(-rectSize/2, -rectSize/2, rectSize, rectSize);
}

BezierPoint::~BezierPoint()
{

}

