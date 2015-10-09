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
    serializeInfo->addValue("name", mName);
    for(Page* p : mPages)  {
        serializeInfo->addChild(p->serialize());
    }
    return serializeInfo;
}

void Blueprint::deserialize(const SerializeInfo& serializeInfo)
{
    mName = serializeInfo.value("name").toString();
    for(auto child : serializeInfo.children()) {
        Page* page = new Page();
        page->deserialize(*child);
        addPage(page);
    }
}

