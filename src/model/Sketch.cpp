#include "Sketch.h"

#include "SketchItem.h"

Sketch::Sketch(GraphicalItem* parentItem)
    : GraphicalItem(GraphicalType::CANVAS, parentItem)
{

}

Sketch::~Sketch()
{

}
