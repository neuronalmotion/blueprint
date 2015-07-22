#include "SketchItemEllipse.h"

#include <QGraphicsEllipseItem>

SketchItemEllipse::SketchItemEllipse(qreal x, qreal y)
    : SketchItemBezier(x, y)
{
    addPath(QPointF(30, 0), QPointF(50, 20), QPointF(50, 50));
    addPath(QPointF(50, 80), QPointF(30, 100), QPointF(0, 100));
    addPath(QPointF(-30, 100), QPointF(-40, 80), QPointF(-40, 50));
    addPath(QPointF(-40, 20), QPointF(-30, 0), QPointF(0, 0));
    closePath();
}

SketchItemEllipse::~SketchItemEllipse()
{

}

void SketchItemEllipse::boundBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta)
{

}

