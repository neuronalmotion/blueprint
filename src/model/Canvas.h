#ifndef CANVAS_H
#define CANVAS_H

#include <QList>

#include "model/ShapeRectangle.h"
#include "model/GraphicalItem.h"

class Shape;

class Canvas : public ShapeRectangle
{
public:
    Canvas(GraphicalItem* parentItem = 0);
    ~Canvas();

};

#endif // CANVAS_H
