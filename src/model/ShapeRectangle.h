#ifndef SHAPERECTANGLE_H
#define SHAPERECTANGLE_H

#include "ShapeBezier.h"

namespace blueprint {

class ShapeRectangle : public ShapeBezier
{
public:
    ShapeRectangle(Shape* parentShape, const qreal& x, const qreal& y);
    ~ShapeRectangle();

    void collapse() override;

protected:
    ShapeRectangle(Shape* parentShape, const ShapeType& shapeType, const qreal& x, const qreal& y);

private:
    void init();
};
}

#endif // SHAPERECTANGLE_H
