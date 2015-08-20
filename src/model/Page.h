#ifndef PAGE_H
#define PAGE_H

#include <QList>

#include "model/TreeItem.h"

namespace blueprint {
class Canvas;

class Page : public TreeItem
{
public:
    Page(TreeItem* parentTreeItem = 0);
    ~Page();

};
}

#endif // PAGE_H
