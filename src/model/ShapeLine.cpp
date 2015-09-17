#include "ShapeLine.h"

using namespace blueprint;

ShapeLine::ShapeLine(blueprint::TreeItem* parentTreeItem, qreal x, qreal y)
    : Shape(parentTreeItem, ShapeType::LINE, x, y)
{
    addPath(QPointF(10, 0), QPointF(20, 0), QPointF(30, 0));

    setBackgroundColor(QColor(0, 0, 0, 0));
    mEditMode = Shape::EditMode::BEZIER;
}

ShapeLine::~ShapeLine()
{

}

void ShapeLine::setEditMode(const Shape::EditMode& mode)
{
    // No op
}
