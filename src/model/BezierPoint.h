#ifndef BEZIERPOINT_H
#define BEZIERPOINT_H

#include <QGraphicsEllipseItem>

#include "BezierElement.h"

class SketchItemBezier;

class BezierPoint : public BezierElement, public QGraphicsEllipseItem
{
public:
    BezierPoint(SketchItemBezier* item, const int index);
    ~BezierPoint();

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
};

#endif // BEZIERPOINT_H
