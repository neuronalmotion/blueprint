#ifndef SKETCHITEMCIRCLE_H
#define SKETCHITEMCIRCLE_H

#include "SketchItemBezier.h"


class SketchItemEllipse : public SketchItemBezier
{
public:
    SketchItemEllipse(qreal x, qreal y);
    ~SketchItemEllipse();

};

#endif // SKETCHITEMCIRCLE_H
