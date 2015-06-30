#ifndef SKETCHITEMBEZIER_H
#define SKETCHITEMBEZIER_H

#include "SketchItem.h"
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>

class QPointF;
class BezierControlPoint;

class SketchItemBezier : public SketchItem
{
public:
    SketchItemBezier();
    ~SketchItemBezier();
    virtual QGraphicsItem* getGraphicsItem();
    void addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos);
    void updateElement(int index, const QPointF& pos);

private:
    QGraphicsPathItem* mItem;
    QPainterPath mPath;
    QGraphicsEllipseItem* mStartPoint;
    QGraphicsEllipseItem* mEndPoint;
    BezierControlPoint* mControl1;
    BezierControlPoint* mControl2;
};

#endif // SKETCHITEMBEZIER_H
