#include "Canvas.h"

#include "Shape.h"

Canvas::Canvas(GraphicalItem* parentItem, qreal x, qreal y)
    : ShapeRectangle(parentItem, x, y)
{

}

Canvas::~Canvas()
{

}
