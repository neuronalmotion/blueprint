#include "BezierPoint.h"

#include <QPen>

#include "SketchItemBezier.h"

BezierPoint::BezierPoint(SketchItemBezier* item, const int index)
    : QGraphicsEllipseItem(item->getGraphicsItem()),
    mItem(item),
    mIndex(index)
{
    setPen(QPen(Qt::red));
}

BezierPoint::~BezierPoint()
{

}

