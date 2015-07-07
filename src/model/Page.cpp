#include "Page.h"

#include "Sketch.h"

Page::Page()
{

}

Page::~Page()
{
    for (auto s : mSketches) {
        delete s;
        s = nullptr;
    }
    mSketches.clear();
}

void Page::addSketch(Sketch* sketch)
{
    mSketches.append(sketch);
}

