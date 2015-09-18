#ifndef SHAPEELLIPSE_H
#define SHAPEELLIPSE_H

#include "ShapeBezier.h"

namespace blueprint {
class ShapeEllipse : public ShapeBezier
{
public:
    ShapeEllipse(TreeItem* parentTreeItem, qreal x, qreal y);
    ~ShapeEllipse();

    void collapse() override;
};
}

#endif // SHAPEELLIPSE_H
