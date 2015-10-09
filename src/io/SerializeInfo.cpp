#include "SerializeInfo.h"

using namespace blueprint;

SerializeInfo::SerializeInfo(const QString& name)
    : mName(name),
      mData()
{

}

SerializeInfo::~SerializeInfo()
{
    qDeleteAll(mChildren);
}

void SerializeInfo::addValue(const QString& key, const QVariant& value)
{
    mData[key] = value;
}

QVariant SerializeInfo::value(const QString& key) const
{
    return mData[key];
}

QMapIterator<QString, QVariant> SerializeInfo::iterator() const
{
   return QMapIterator<QString, QVariant>(mData);
}


