#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QHash>

#include "BoundingBoxPoint.h"

class Shape;

struct BoundingBoxEvent
{
    QPointF origin;
    QPointF delta;
    QPointF scale;
};

class BoundingBox : public QGraphicsRectItem
{
public:
    BoundingBox(Shape* parentSketchItem);
    ~BoundingBox();

    void updateRect(BoundingBoxPoint::TranslationDirection ignoredDirection = BoundingBoxPoint::NONE);
    void boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta);
    Shape* getParentSketchItem() const { return mParentSketchItem; }

private:
    Shape* mParentSketchItem;
    BoundingBoxEvent mBoundingBoxEvent;

    QHash<BoundingBoxPoint::TranslationDirection, BoundingBoxPoint*> mHashBoundingBoxPoints;

};

#endif // BOUNDINGBOX_H
