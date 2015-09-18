#ifndef SHAPERECTANGLE_H
#define SHAPERECTANGLE_H

#include "ShapeBezier.h"

namespace blueprint {

class ShapeRectangle : public ShapeBezier
{
public:
    ShapeRectangle(TreeItem* parentTreeItem, qreal x, qreal y);
    ~ShapeRectangle();

    void collapse() override;
};
}

#endif // SHAPERECTANGLE_H
