#include "ShapeLine.h"

using namespace blueprint;

ShapeLine::ShapeLine(Shape* parentShape, const qreal& x, const qreal& y)
    : ShapeBezier(parentShape, ShapeType::LINE, x, y)
{
    addSegment(QPointF(10, 10));
    setBackgroundColor(QColor(0, 0, 0, 0));
    mEditMode = Shape::EditMode::BEZIER;
}

ShapeLine::~ShapeLine()
{

}

void ShapeLine::setEditMode(const Shape::EditMode& /*mode*/)
{
    // no-op
}

void ShapeLine::collapse()
{
    // no-op
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

    // hide control points
    mElements[1]->setVisible(false);
    mElements[mElements.length() - 2]->setVisible(false);
}
