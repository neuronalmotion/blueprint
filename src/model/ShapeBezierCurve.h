#ifndef SHAPEBEZIERCURVE_H
#define SHAPEBEZIERCURVE_H

#include "ShapeBezier.h"

namespace blueprint {
class ShapeBezierCurve : public ShapeBezier
{
public:
    ShapeBezierCurve(Shape* parentShape);
    ~ShapeBezierCurve();

    void setEditMode(const EditMode& mode) override;
    void resizeOnCreation(const QPointF& delta) override;

protected:
    void updateBoundingBoxBezierVisibility() override;
};
}

#endif // SHAPEBEZIERCURVE_H
