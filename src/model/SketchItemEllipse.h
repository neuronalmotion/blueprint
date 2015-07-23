#ifndef SKETCHITEMCIRCLE_H
#define SKETCHITEMCIRCLE_H

#include "SketchItemBezier.h"


class SketchItemEllipse : public SketchItemBezier
{
public:
    SketchItemEllipse(qreal x, qreal y);
    ~SketchItemEllipse();

    virtual void boundBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta);
};

#endif // SKETCHITEMCIRCLE_H
