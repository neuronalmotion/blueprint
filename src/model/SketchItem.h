#ifndef SKETCHITEM_H
#define SKETCHITEM_H

#include <QGraphicsItem>

#include "model/BoundingBoxPoint.h"

class SketchItem
{
public:
    SketchItem();
    virtual ~SketchItem();
    virtual QGraphicsItem* getGraphicsItem() = 0;

    virtual void boundBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta) = 0;

    QString name;

};

#endif // SKETCHITEM_H
