#ifndef SKETCHITEMCIRCLE_H
#define SKETCHITEMCIRCLE_H

#include "SketchItem.h"
#include <QGraphicsItemGroup>


class SketchItemEllipse : public SketchItem
{
public:
    SketchItemEllipse();
    ~SketchItemEllipse();
    virtual QGraphicsItem* getGraphicsItem();

private:
    QGraphicsItemGroup* mItem;
};

#endif // SKETCHITEMCIRCLE_H
