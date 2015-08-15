#include "ShapeRectangle.h"

#include <QDebug>

// Constants
static const uint TOP_LEFT_INDEX        = 12;
static const uint TOP_RIGHT_INDEX       = 3;
static const uint BOTTOM_RIGHT_INDEX    = 6;
static const uint BOTTOM_LEFT_INDEX     = 9;

ShapeRectangle::ShapeRectangle(qreal x, qreal y)
    : ShapeBezier(x, y)
{
    addPath(QPointF(40, 0), QPointF(60, 0), QPointF(100, 0));
    addPath(QPointF(100, 20), QPointF(100, 40), QPointF(100, 50));
    addPath(QPointF(60, 50), QPointF(40, 50), QPointF(0, 50));
    addPath(QPointF(0, 40), QPointF(0, 20), QPointF(0, 0));
    closePath();
    mBoundingBox->updateRect();
}

