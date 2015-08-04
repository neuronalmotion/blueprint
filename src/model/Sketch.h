#ifndef SKETCH_H
#define SKETCH_H

#include <QList>

#include "model/GraphicalItem.h"

class SketchItem;

class Sketch : public GraphicalItem
{
public:
    Sketch(GraphicalItem* parentItem = 0);
    ~Sketch();

};

#endif // SKETCH_H
