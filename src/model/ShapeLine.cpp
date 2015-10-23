#include "ShapeLine.h"

using namespace blueprint;

ShapeLine::ShapeLine(Shape* parentShape)
    : ShapeBezier(parentShape, ShapeType::LINE, 0, 0)
{
    addSegment(QPointF(10, 10));
    setForegroundColor(QColor(0, 0, 0, 0));
    mEditMode = Shape::EditMode::PATH;
}

ShapeLine::~ShapeLine()
{

}

void ShapeLine::setEditMode(const Shape::EditMode& /*mode*/)
{
    // no-op
}

void ShapeLine::resizeOnCreation(const QPointF& delta)
{
    mElements.last()->moveBy(delta);
}

void ShapeLine::updateBoundingBoxBezierVisibility()
{
    bool bezierVisibility = mIsSelected && mEditMode == EditMode::PATH;

    for (auto e : mElements) {
        e->setVisible(bezierVisibility);
    }

    // hide control points
    mElements[1]->setVisible(false);
    mElements[mElements.length() - 2]->setVisible(false);
}
