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
    BezierPoint* mEndPoint;
    BezierControlPoint* mControl1;
    BezierControlPoint* mControl2;

};

#endif // BEZIERPATH_H
