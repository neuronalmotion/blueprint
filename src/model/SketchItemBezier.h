#ifndef SKETCHITEMBEZIER_H
#define SKETCHITEMBEZIER_H

#include "SketchItem.h"
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

class SketchItemBezier : public SketchItem
{
public:
    SketchItemBezier();
    ~SketchItemBezier();
    virtual QGraphicsItem* getGraphicsItem();

private:
    QGraphicsPathItem* mItem;
    QGraphicsEllipseItem* mStartPoint;
    QGraphicsEllipseItem* mEndPoint;
    QGraphicsRectItem* mControl1;
    QGraphicsRectItem* mControl2;
};

#endif // SKETCHITEMBEZIER_H
