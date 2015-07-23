#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QGraphicsRectItem>
#include <QPointF>

#include "BoundingBoxPoint.h"

class SketchItem;

class BoundingBox : public QGraphicsRectItem
{
public:
    const int HANDLE_SIZE = 10;
    BoundingBox(SketchItem* parentSketchItem);
    ~BoundingBox();

    void updateRect(BoundingBoxPoint::TranslationDirection ignoredDirection = BoundingBoxPoint::NONE);
    void boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta);

private:
    SketchItem* mParentSketchItem;

    BoundingBoxPoint mTopLeftHandle;
    BoundingBoxPoint mTopHandle;
    BoundingBoxPoint mTopRightHandle;
    BoundingBoxPoint mRightHandle;
    BoundingBoxPoint mBottomRight;
    BoundingBoxPoint mBottom;
    BoundingBoxPoint mBottomLeft;
    BoundingBoxPoint mLeft;

};

#endif // BOUNDINGBOX_H
