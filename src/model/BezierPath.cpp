#include "BezierPath.h"

#include "SketchItemBezier.h"
#include <QPainterPath>

BezierPath::BezierPath(SketchItemBezier* item, const QPainterPath* path, const int elementStartIndex)
    : mItem(item),
    mElementStartIndex(elementStartIndex),
    mStartPoint(new BezierPoint(item, elementStartIndex)),
    mEndPoint(new BezierPoint(item, elementStartIndex + 3)),
    mControl1(new BezierControlPoint(item, elementStartIndex + 1)),
    mControl2(new BezierControlPoint(item, elementStartIndex + 2))
{
    QPainterPath::Element element = path->elementAt(elementStartIndex);
    mStartPoint->setPos(element.x, element.y);

    element = path->elementAt(elementStartIndex + 1);
    mControl1->setPos(element.x, element.y);

    element = path->elementAt(elementStartIndex + 2);
    mControl2->setPos(element.x, element.y);

    element = path->elementAt(elementStartIndex + 3);
    mEndPoint->setPos(element.x, element.y);
}


BezierPath::~BezierPath()
{
    delete mStartPoint;
    delete mEndPoint;
    delete mControl1;
    delete mControl2;
}

