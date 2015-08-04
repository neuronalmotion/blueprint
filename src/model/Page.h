#ifndef PAGE_H
#define PAGE_H

#include <QList>

#include "model/GraphicalItem.h"

class Sketch;

class Page : public GraphicalItem
{
public:
    Page(GraphicalItem* parentItem = 0);
    ~Page();

};

#endif // PAGE_H
