#include "Blueprint.h"

using namespace blueprint;

Blueprint::Blueprint()
    : mName("Blueprint project"),
      mPages()
{

}

Blueprint::~Blueprint()
{
}

SerializeInfo* Blueprint::serialize() const
{
    SerializeInfo* serializeInfo = new SerializeInfo("blueprint");
    serializeInfo->addValue("name", mName);
    for(Page* p : mPages)  {
        serializeInfo->addChild(p->serialize());
    }
    return serializeInfo;
}

void Blueprint::deserialize(const SerializeInfo& serializeInfo)
{

}

