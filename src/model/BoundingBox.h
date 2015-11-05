#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QHash>

#include "BoundingBoxPoint.h"

namespace blueprint {
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
    BoundingBox(Shape* parent);
    ~BoundingBox();

    void updateRect(BoundingBoxPoint::TranslationDirection ignoredDirection = BoundingBoxPoint::NONE);
    void boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta);
    Shape* parentShape() const { return mParentShape; }
    const BoundingBoxPoint* boundingBoxPoint(BoundingBoxPoint::TranslationDirection direction) const;

    void updateParentGraphicsItem();

private:
    Shape* mParentShape;
    BoundingBoxEvent mBoundingBoxEvent;
    QHash<BoundingBoxPoint::TranslationDirection, BoundingBoxPoint*> mHashBoundingBoxPoints;

};
}

#endif // BOUNDINGBOX_H
