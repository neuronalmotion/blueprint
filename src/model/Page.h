#ifndef PAGE_H
#define PAGE_H

#include <QList>

#include "model/TreeItem.h"

class Canvas;

class Page : public TreeItem
{
public:
    Page(TreeItem* parentItem = 0);
    ~Page();

};

#endif // PAGE_H
