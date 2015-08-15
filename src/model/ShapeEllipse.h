#ifndef SHAPEELLIPSE_H
#define SHAPEELLIPSE_H

#include "ShapeBezier.h"


class ShapeEllipse : public ShapeBezier
{
public:
    ShapeEllipse(GraphicalItem* parentItem, qreal x, qreal y);
    ~ShapeEllipse();

};

#endif // SHAPEELLIPSE_H
