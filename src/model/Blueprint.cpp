#include "Blueprint.h"

#include "model/Page.h"

Blueprint::Blueprint()
{

}

Blueprint::~Blueprint()
{
    for (auto p : mPages)  {
        delete p;
        p = nullptr;
    }
    mPages.clear();
}

void Blueprint::addPage(Page *page)
{
    mPages.append(page);
}

