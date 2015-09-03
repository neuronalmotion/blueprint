#ifndef SHAPEELLIPSE_H
#define SHAPEELLIPSE_H

#include "Shape.h"

namespace blueprint {
class ShapeEllipse : public Shape
{
public:
    ShapeEllipse(TreeItem* parentTreeItem, qreal x, qreal y);
    ~ShapeEllipse();

    void collapse() override;
};
}

#endif // SHAPEELLIPSE_H
