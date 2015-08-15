#ifndef BEZIERCONTROLPOINT_H
#define BEZIERCONTROLPOINT_H

#include <QGraphicsRectItem>

#include "BezierElement.h"


class ShapeBezier;

class BezierControlPoint : public BezierElement, public QGraphicsRectItem
{
public:
    BezierControlPoint(ShapeBezier* parent, const int index);
    ~BezierControlPoint();

    QPointF getPos();
    void setPos(QPointF pos);
    void moveBy(QPointF delta);
    void setVisible(bool isVisible);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // BEZIERCONTROLPOINT_H
