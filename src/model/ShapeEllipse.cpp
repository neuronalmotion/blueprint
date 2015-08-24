#include "ShapeEllipse.h"

#include <QDebug>
#include <QGraphicsEllipseItem>

using namespace blueprint;

ShapeEllipse::ShapeEllipse(TreeItem* parentItem, qreal x, qreal y)
    : Shape(parentItem, x, y)
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
