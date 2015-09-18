#include "ShapeLine.h"

using namespace blueprint;

ShapeLine::ShapeLine(blueprint::TreeItem* parentTreeItem, qreal x, qreal y)
    : ShapeBezier(parentTreeItem, ShapeType::LINE, x, y)
{
    addPath(QPointF(10, 0), QPointF(20, 0), QPointF(30, 0));

    setBackgroundColor(QColor(0, 0, 0, 0));
    mEditMode = Shape::EditMode::BEZIER;
}

ShapeLine::~ShapeLine()
{

}

void ShapeLine::setEditMode(const Shape::EditMode& /*mode*/)
{
    // No op
}

void ShapeLine::collapse()
{
    for (auto e : mElements) {
        e->setPos(QPointF(0, 0));
    }
}

void ShapeLine::resizeOnCreation(const QPointF& delta)
{
    mElements.last()->moveBy(delta);
}

void ShapeLine::updateBoundingBoxBezierVisibility()
{
    bool bezierVisibility = mIsSelected && mEditMode == EditMode::BEZIER;

    for (auto e : mElements) {
        e->setVisible(bezierVisibility);
    }

    mElements[1]->setVisible(false);
    mElements[mElements.length() - 2]->setVisible(false);
}
