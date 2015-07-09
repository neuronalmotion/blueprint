#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "BezierPoint.h"
#include "BezierControlPoint.h"
#include "SketchItemBezier.h"

class QPainterPath;

class BezierPath
{
public:
    BezierPath(SketchItemBezier* item, const QPainterPath* path, const int elementStartIndex);
    ~BezierPath();

    void updateElement(BezierElement* bezierElement, const QPointF& pos);
    inline QGraphicsItem* getGraphicsItem() { return mItem->getGraphicsItem(); }

    inline bool hasNextPath() { return mNextPath != nullptr; }
    inline void setNextPath(BezierPath* path) { mNextPath = path; }

private:
    SketchItemBezier* mItem;
    int mElementStartIndex;
    BezierPoint* mStartPoint;
    BezierControlPoint* mControl1;
    BezierControlPoint* mControl2;
    BezierPoint* mEndPoint;
    BezierPath* mNextPath;
};

#endif // BEZIERPATH_H
