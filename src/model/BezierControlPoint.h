#ifndef BEZIERCONTROLPOINT_H
#define BEZIERCONTROLPOINT_H

#include <QGraphicsRectItem>

#include "BezierElement.h"


class SketchItemBezier;

class BezierControlPoint : public BezierElement, public QGraphicsRectItem
{
public:
    BezierControlPoint(SketchItemBezier* parent, const int index);
    ~BezierControlPoint();

    QPointF getPos();
    void setPos(QPointF pos);
    void moveBy(QPointF delta);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // BEZIERCONTROLPOINT_H
