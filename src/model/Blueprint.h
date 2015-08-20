#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <QList>

#include "model/TreeItem.h"

namespace blueprint {

class Page;

class Blueprint : public TreeItem
{
public:
    Blueprint();
    ~Blueprint();
};

}

#endif // BLUEPRINT_H
