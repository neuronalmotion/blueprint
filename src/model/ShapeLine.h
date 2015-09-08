#ifndef SHAPELINE_H
#define SHAPELINE_H

#include "Shape.h"

namespace blueprint {
class ShapeLine : public Shape
{
public:
    ShapeLine(TreeItem* parentTreeItem, qreal x, qreal y);
    ~ShapeLine();
};
}

#endif // SHAPELINE_H
