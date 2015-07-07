#include "Sketch.h"

#include "SketchItem.h"

Sketch::Sketch()
{

}

Sketch::~Sketch()
{

}

void Sketch::addSketchItem(SketchItem* item)
{
    mItems.append(item);
}

