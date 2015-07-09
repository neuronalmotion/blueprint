#ifndef BEZIERPOINT_H
#define BEZIERPOINT_H

#include <QGraphicsEllipseItem>

#include "BezierElement.h"

class SketchItemBezier;

class BezierPoint : public BezierElement, public QGraphicsEllipseItem
{
public:
    BezierPoint(BezierPath* parent, const int index);
    ~BezierPoint();

    QPointF getPos();

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
};

#endif // BEZIERPOINT_H
