#include "Blueprint.h"

#include "ShapeFactory.h"

using namespace blueprint;

Blueprint::Blueprint()
    : mName("Blueprint project"),
      mActivePage(nullptr),
      mPages(),
      mFile(nullptr)
{

}

Blueprint::~Blueprint()
{
    delete mFile;
    qDeleteAll(mPages);
}

void Blueprint::addPage(Page* page)
{
    mPages.append(page);
}

bool Blueprint::hasFile() const
{
    return mFile && mFile->exists();
}

void Blueprint::setFile(const QString& filepath)
{
    if (!mFile) {
        mFile = new QFile(filepath);
    }
    mFile->setFileName(filepath);
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
            if (!activePage()) {
                setActivePage(page);
            }
        }
    }
}
