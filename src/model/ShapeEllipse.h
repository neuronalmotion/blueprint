#ifndef SHAPEELLIPSE_H
#define SHAPEELLIPSE_H

#include "Shape.h"

namespace blueprint {
class ShapeEllipse : public Shape
{
public:
    ShapeEllipse(TreeItem* parentItem, qreal x, qreal y);
    ~ShapeEllipse();

};
}

#endif // SHAPEELLIPSE_H
