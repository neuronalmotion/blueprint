#include "ShapeBezierCurve.h"

using namespace blueprint;

ShapeBezierCurve::ShapeBezierCurve(Shape* parentShape)
    : ShapeBezier(parentShape, ShapeType::BEZIER_CURVE, 0, 0)
{
    addPath(QPointF(0, 0), QPointF(0, 0), QPointF(0, 0));
    setForegroundColor(QColor(0, 0, 0, 0));
    mEditMode = Shape::EditMode::PATH;
}

ShapeBezierCurve::~ShapeBezierCurve()
{

}

void ShapeBezierCurve::setEditMode(const Shape::EditMode& /*mode*/)
{
    // no-op
}

void ShapeBezierCurve::resizeOnCreation(const QPointF& delta)
{
    //mElements.last()->moveBy(delta);
}

void ShapeBezierCurve::updateBoundingBoxBezierVisibility()
{
    bool bezierVisibility = mIsSelected && mEditMode == EditMode::PATH;

    for (auto e : mElements) {
        e->setVisible(bezierVisibility);
    }

    // hide control points
    //mElements[1]->setVisible(false);
    //mElements[mElements.length() - 2]->setVisible(false);
}
