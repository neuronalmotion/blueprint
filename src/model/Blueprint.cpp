#include "Blueprint.h"

#include "ShapeFactory.h"

using namespace blueprint;

Blueprint::Blueprint()
    : mName("Blueprint project"),
      mActivePage(nullptr),
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
