#ifndef SHAPERECTANGLE_H
#define SHAPERECTANGLE_H

#include "Shape.h"

namespace blueprint {

class ShapeRectangle : public Shape
{
public:
    ShapeRectangle(TreeItem* parentItem, qreal x, qreal y);
    ~ShapeRectangle();

};
}

#endif // SHAPERECTANGLE_H
