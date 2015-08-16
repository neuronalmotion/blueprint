#ifndef BEZIERPOINT_H
#define BEZIERPOINT_H

#include <QGraphicsEllipseItem>

#include "BezierElement.h"

class ShapeBezier;

class BezierPoint : public BezierElement, public QGraphicsEllipseItem
{
public:
    BezierPoint(ShapeBezier* parent, const int index);
    ~BezierPoint();

    QPointF getPos();
    void setPos(QPointF pos);
    void moveBy(QPointF delta);
    void setVisible(bool isVisible);

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
};

#endif // BEZIERPOINT_H
