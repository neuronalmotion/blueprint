#include "ShapeRectangle.h"

#include <QDebug>

using namespace blueprint;

ShapeRectangle::ShapeRectangle(TreeItem* parentItem, qreal x, qreal y)
    : Shape(parentItem, ShapeType::RECTANGLE, x, y)
{
    addPath(QPointF(40, 0), QPointF(60, 0), QPointF(100, 0));
    addPath(QPointF(100, 20), QPointF(100, 40), QPointF(100, 50));
    addPath(QPointF(60, 50), QPointF(40, 50), QPointF(0, 50));
    addPath(QPointF(0, 40), QPointF(0, 20), QPointF(0, 0));
    closePath();
    mBoundingBox.updateRect();
}

ShapeRectangle::~ShapeRectangle()
{
}

void ShapeRectangle::collapse()
{
    mBoundingBox.boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection::BOTTOM_RIGHT, QPointF(-99, -49));
}

