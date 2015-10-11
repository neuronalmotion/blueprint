#include "SerializeInfo.h"

using namespace blueprint;

SerializeInfo::Type SerializeInfo::stringToType(const QString& string)
{
    Type type;
    if (string == "property" || string.isEmpty()) {
         type = VALUE;
    } else if (string == "list") {
        type = LIST;

    } else if (string == "object") {
        type = OBJECT;
    }
    return type;
}

QString SerializeInfo::typeToString(const SerializeInfo::Type& type)
{
    QString string;
    switch (type) {
    case VALUE:
        string = "";
    break;
    case LIST:
        string = "list";
    break;
    case OBJECT:
        string = "object";
    break;
    default:
    case UNDEFINED:
        string = "undefined";
    break;
    }
    return string;
}

SerializeInfo::SerializeInfo(const QString& name)
    : mName(name),
      mValue(),
      mList(),
      mProperties()
{

}

SerializeInfo::~SerializeInfo()
{
    qDeleteAll(mList);
    qDeleteAll(mProperties);
}

void SerializeInfo::setValue(const QVariant& value)
{
    mValue = value;
}

void SerializeInfo::putProperty(const QString& key, const QVariant& value)
{
    SerializeInfo* serializeInfo = new SerializeInfo(key);
    serializeInfo->setValue(value);
    mProperties[key] = serializeInfo;
}

void SerializeInfo::putProperty(const QString& key, SerializeInfo* info)
{
    mProperties[key] = info;
}

void SerializeInfo::addPropertyToKey(const QString& key, SerializeInfo* info)
{
    SerializeInfo* serializeInfo = nullptr;
    if (mProperties.contains(key)) {
        serializeInfo = mProperties[key];
    } else {
        serializeInfo = new SerializeInfo(key);
        mProperties[key] = serializeInfo;
    }
    serializeInfo->addElement(info);
}

SerializeInfo*SerializeInfo::at(const QString& key) const
{
    return mProperties[key];
}

QVariant SerializeInfo::propertyValue(const QString& key) const
{
    return at(key)->value();
}

QMapIterator<QString, SerializeInfo*> SerializeInfo::propertiesIterator() const
{
    return QMapIterator<QString, SerializeInfo*>(mProperties);
}

SerializeInfo::Type SerializeInfo::type() const
{
    if (!mValue.isNull()
            && mList.isEmpty()
            && mProperties.isEmpty()) {
        return VALUE;
    } else if (mValue.isNull()
            && !mList.isEmpty()
            && mProperties.isEmpty()) {
        return LIST;
    } else if (mValue.isNull()
            && mList.isEmpty()
            && !mProperties.isEmpty()) {
        return OBJECT;
    }
    return UNDEFINED;

}


