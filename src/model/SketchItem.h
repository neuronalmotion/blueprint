#ifndef SKETCHITEM_H
#define SKETCHITEM_H

#include <QGraphicsItem>

class SketchItem
{
public:
    SketchItem();
    virtual ~SketchItem();
    virtual QGraphicsItem* getGraphicsItem() = 0;

};

#endif // SKETCHITEM_H
