#ifndef SKETCHITEMBEZIER_H
#define SKETCHITEMBEZIER_H

#include "SketchItem.h"
#include <QList>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>

class QPointF;
class BezierPath;

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
    QList<BezierPath*> mPathes;
};

#endif // SKETCHITEMBEZIER_H
