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

    void updateRect();

signals:
    void resized(QPointF top, QPointF right, QPointF bottom, QPointF left);

private:
    SketchItem* mParentSketchItem;

    BoundingBoxPoint mTopHandle;

};

#endif // BOUNDINGBOX_H
