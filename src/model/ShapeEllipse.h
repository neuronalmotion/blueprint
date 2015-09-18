#ifndef SHAPEELLIPSE_H
#define SHAPEELLIPSE_H

#include "ShapeBezier.h"

namespace blueprint {
class ShapeEllipse : public ShapeBezier
{
public:
    ShapeEllipse(Shape* parentShape, const qreal& x, const qreal& y);
    ~ShapeEllipse();

    void collapse() override;
};
}

#endif // SHAPEELLIPSE_H
