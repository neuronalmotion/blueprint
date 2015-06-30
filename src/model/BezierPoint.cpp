#include "BezierPoint.h"

#include <QPen>

#include "SketchItemBezier.h"

BezierPoint::BezierPoint(SketchItemBezier* item)
    : QGraphicsEllipseItem(item->getGraphicsItem()),
    mItem(item)
{
    setPen(QPen(Qt::red));
}

BezierPoint::~BezierPoint()
{

}

