#ifndef SKETCHITEMBEZIER_H
#define SKETCHITEMBEZIER_H

#include "SketchItem.h"

#include <QList>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>

class QPointF;
class BezierPath;
class BezierElement;

class SketchItemBezier : public SketchItem
{
public:
    SketchItemBezier();
    ~SketchItemBezier();
    virtual QGraphicsItem* getGraphicsItem();
    void addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos);
    void closePath();
    void updateElement(BezierElement* bezierElement, const QPointF& pos);

    QPainterPath mPath;
private:
    QGraphicsPathItem* mItem;
    QList<BezierElement*> mElements;
    bool mIsPathClosed;
};

#endif // SKETCHITEMBEZIER_H
