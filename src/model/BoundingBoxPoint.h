#ifndef BOUNDINGBOXPOINT_H
#define BOUNDINGBOXPOINT_H

#include <QGraphicsRectItem>

class QGraphicsSceneDragDropEvent;

class BoundingBoxPoint : public QGraphicsRectItem
{
public:
    BoundingBoxPoint(QGraphicsItem* parent=0);
    ~BoundingBoxPoint();

protected:
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
};

#endif // BOUNDINGBOXPOINT_H
