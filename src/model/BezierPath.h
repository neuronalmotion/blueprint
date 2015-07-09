#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "BezierPoint.h"
#include "BezierControlPoint.h"

class SketchItemBezier;
class QPainterPath;

class BezierPath
{
public:
    BezierPath(SketchItemBezier* item, const QPainterPath* path, const int elementStartIndex);
    ~BezierPath();

private:
    SketchItemBezier* mItem;
    int mElementStartIndex;
    BezierPoint* mStartPoint;
    BezierControlPoint* mControl1;
    BezierControlPoint* mControl2;
    BezierPoint* mEndPoint;
};

#endif // BEZIERPATH_H
