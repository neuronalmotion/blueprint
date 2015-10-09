#include "SerializeInfo.h"

using namespace blueprint;

SerializeInfo::SerializeInfo(const QString& name)
    : mName(name),
      mProperties()
{

}

SerializeInfo::~SerializeInfo()
{
    qDeleteAll(mChildren);
}

void SerializeInfo::addValue(const QString& key, const QVariant& value)
{
    mProperties[key] = value;
}

QVariant SerializeInfo::value(const QString& key) const
{
    return mProperties[key];
}

QMapIterator<QString, QVariant> SerializeInfo::iterator() const
{
   return QMapIterator<QString, QVariant>(mProperties);
}


