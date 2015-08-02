#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QHash>

#include "BoundingBoxPoint.h"

class SketchItem;

struct BoundingBoxEvent
{
    QPointF origin;
    QPointF delta;
    QPointF scale;
};

class BoundingBox : public QGraphicsRectItem
{
public:
    BoundingBox(SketchItem* parentSketchItem);
    ~BoundingBox();

    void updateRect(BoundingBoxPoint::TranslationDirection ignoredDirection = BoundingBoxPoint::NONE);
    void boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta);
    SketchItem* getParentSketchItem() const { return mParentSketchItem; }

private:
    SketchItem* mParentSketchItem;
    BoundingBoxEvent mBoundingBoxEvent;

    QHash<BoundingBoxPoint::TranslationDirection, BoundingBoxPoint*> mHashBoundingBoxPoints;

};

#endif // BOUNDINGBOX_H
