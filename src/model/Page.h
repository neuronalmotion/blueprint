#ifndef PAGE_H
#define PAGE_H

#include <QList>

class Sketch;

class Page
{
public:
    Page();
    ~Page();

private:
    QList<Sketch*> mSketches;
};

#endif // PAGE_H
