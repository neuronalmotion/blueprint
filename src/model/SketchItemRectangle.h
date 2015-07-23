#ifndef SKETCHITEMRECTANGLE_H
#define SKETCHITEMRECTANGLE_H

#include "SketchItemBezier.h"


class SketchItemRectangle : public SketchItemBezier
{
public:
    SketchItemRectangle(qreal x, qreal y);

    virtual void boundBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta);
};

#endif // SKETCHITEMRECTANGLE_H
