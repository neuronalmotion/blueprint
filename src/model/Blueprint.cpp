#include "Blueprint.h"

#include "ShapeFactory.h"

using namespace blueprint;

Blueprint::Blueprint()
    : mName("Blueprint project"),
      mPages()
{

}

Blueprint::~Blueprint()
{
    qDeleteAll(mPages);
}

void Blueprint::addPage(Page* page)
{
    mPages.append(page);

    if (mPages.count() == 1)
    {

    }
}

Parcel* Blueprint::toParcel() const
{
    Parcel* parcel = new Parcel("blueprint");
    parcel->putProperty("name", mName);
    for(Page* p : mPages)  {
        parcel->addPropertyToKey("children", p->toParcel());
    }
    return parcel;
}

void Blueprint::fromParcel(const Parcel& parcel)
{
    qDeleteAll(mPages);
    mName = parcel.propertyValue("name").toString();
    if (parcel.contains("children")) {
        Parcel* children = parcel.at("children");
        for(auto child : children->list()) {
            Page* page = ShapeFactory::createPage(false);
            page->fromParcel(*child);
            addPage(page);
        }
    }
}

//void Blueprint::addPage(Page* page) { mPages.append(page); }

