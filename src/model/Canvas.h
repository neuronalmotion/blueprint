#ifndef CANVAS_H
#define CANVAS_H

#include <QList>

#include "model/GraphicalItem.h"

class SketchItem;

class Canvas : public GraphicalItem
{
public:
    Canvas(GraphicalItem* parentItem = 0);
    ~Canvas();

};

#endif // CANVAS_H
