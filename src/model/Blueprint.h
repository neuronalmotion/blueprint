#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <QList>

class Page;

class Blueprint
{
public:
    Blueprint();
    ~Blueprint();

    void addPage(Page* page);

private:
    QList<Page*> mPages;
};

#endif // BLUEPRINT_H
