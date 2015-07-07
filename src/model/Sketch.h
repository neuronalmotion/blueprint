#ifndef SKETCH_H
#define SKETCH_H

#include <QList>

class SketchItem;

class Sketch
{
public:
    Sketch();
    ~Sketch();

private:
    QList<SketchItem*> mItems;
};

#endif // SKETCH_H
