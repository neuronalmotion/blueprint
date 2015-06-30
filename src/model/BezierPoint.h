#ifndef BEZIERPOINT_H
#define BEZIERPOINT_H

#include <QGraphicsEllipseItem>

#include "BezierControlPoint.h"

class SketchItemBezier;

class BezierPoint : public QGraphicsEllipseItem
{
public:
    BezierPoint(SketchItemBezier* item);
    ~BezierPoint();

private:
    SketchItemBezier* mItem;
//    BezierControlPoint mControl1;
//    BezierControlPoint mControl2;
};

#endif // BEZIERPOINT_H
