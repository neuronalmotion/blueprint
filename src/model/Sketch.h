#ifndef SKETCH_H
#define SKETCH_H

#include <QList>

class SketchItem;

class Sketch
{
public:
    Sketch();
    ~Sketch();

    void addSketchItem(SketchItem* item);

private:
    QList<SketchItem*> mItems;
};

#endif // SKETCH_H
