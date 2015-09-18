#ifndef SHAPELINE_H
#define SHAPELINE_H

#include "ShapeBezier.h"

namespace blueprint {
class ShapeLine : public ShapeBezier
{
public:
    ShapeLine(TreeItem* parentTreeItem, qreal x, qreal y);
    ~ShapeLine();

    void setEditMode(const EditMode& mode) override;
    void collapse() override;
    void resizeOnCreation(const QPointF& delta) override;

protected:
    void updateBoundingBoxBezierVisibility() override;
};
}

#endif // SHAPELINE_H
