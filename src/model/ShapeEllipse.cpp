#include "ShapeEllipse.h"

#include <QDebug>
#include <QGraphicsEllipseItem>


// Constants
static const uint TOP_INDEX     = 12;
static const uint RIGHT_INDEX   = 3;
static const uint BOTTOM_INDEX  = 6;
static const uint LEFT_INDEX    = 9;

ShapeEllipse::ShapeEllipse(GraphicalItem* parentItem, qreal x, qreal y)
    : ShapeBezier(parentItem, x, y)
{
    addPath(QPointF(30, 0), QPointF(50, 20), QPointF(50, 50));
    addPath(QPointF(50, 80), QPointF(30, 100), QPointF(0, 100));
    addPath(QPointF(-40, 100), QPointF(-50, 80), QPointF(-50, 50));
    addPath(QPointF(-40, 20), QPointF(-30, 0), QPointF(0, 0));
    closePath();
    mBoundingBox->updateRect();
}

ShapeEllipse::~ShapeEllipse()
{

}
