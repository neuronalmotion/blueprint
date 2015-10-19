#ifndef SHAPELINE_H
#define SHAPELINE_H

#include "ShapeBezier.h"

namespace blueprint {
class ShapeLine : public ShapeBezier
{
public:
    ShapeLine(Shape* parentShape);
    ~ShapeLine();

    void setEditMode(const EditMode& mode) override;
    void resizeOnCreation(const QPointF& delta) override;

protected:
    void updateBoundingBoxBezierVisibility() override;
};
}

#endif // SHAPELINE_H
