#include "BezierPath.h"

#include "SketchItemBezier.h"

BezierPath::BezierPath(SketchItemBezier* item, int elementStartIndex)
    : mItem(item),
    mElementStartIndex(elementStartIndex),
    mStartPoint(new BezierPoint(item, elementStartIndex)),
    mEndPoint(new BezierPoint(item, elementStartIndex + 3)),
    mControl1(new BezierControlPoint(item, elementStartIndex + 1)),
    mControl2(new BezierControlPoint(item, elementStartIndex + 2))
{

}

BezierPath::~BezierPath()
{
    delete mStartPoint;
    delete mEndPoint;
    delete mControl1;
    delete mControl2;
}

