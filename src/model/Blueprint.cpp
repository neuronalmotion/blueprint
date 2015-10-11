#include "Blueprint.h"

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

SerializeInfo* Blueprint::serialize() const
{
    SerializeInfo* serializeInfo = new SerializeInfo(IO_NAME_BLUEPRINT);
    serializeInfo->putProperty("name", mName);
    for(Page* p : mPages)  {
        serializeInfo->addPropertyToKey("children", p->serialize());
    }
    return serializeInfo;
}

void Blueprint::deserialize(const SerializeInfo& serializeInfo)
{
    mName = serializeInfo.propertyValue("name").toString();
    SerializeInfo* children = serializeInfo.at("children");
    for(auto child : children->list()) {
        Page* page = new Page();
        page->deserialize(*child);
        addPage(page);
    }
}

