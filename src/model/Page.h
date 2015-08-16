#ifndef PAGE_H
#define PAGE_H

#include <QList>

#include "model/GraphicalItem.h"

class Canvas;

class Page : public GraphicalItem
{
public:
    Page(GraphicalItem* parentItem = 0);
    ~Page();

};

#endif // PAGE_H
