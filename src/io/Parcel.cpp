#include "Parcel.h"

using namespace blueprint;

Parcel::Type Parcel::stringToType(const QString& string)
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

QString Parcel::typeToString(const Parcel::Type& type)
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

Parcel::Parcel(const QString& name)
    : mName(name),
      mValue(),
      mList(),
      mProperties()
{

}

Parcel::~Parcel()
{
    qDeleteAll(mList);
    qDeleteAll(mProperties);
}

void Parcel::setValue(const QVariant& value)
{
    mValue = value;
}

void Parcel::putProperty(const QString& key, const QVariant& value)
{
    Parcel* parcel = new Parcel(key);
    parcel->setValue(value);
    putProperty(key, parcel);
}

void Parcel::putProperty(const QString& key, Parcel* info)
{
    mProperties[key] = info;
}

void Parcel::addPropertyToKey(const QString& key, Parcel* info)
{
    Parcel* parcel = nullptr;
    if (mProperties.contains(key)) {
        parcel = mProperties[key];
    } else {
        parcel = new Parcel(key);
        mProperties[key] = parcel;
    }
    parcel->addElement(info);
}

Parcel*Parcel::at(const QString& key) const
{
    return mProperties[key];
}

bool Parcel::contains(const QString& key) const
{
    return mProperties.contains(key);
}

QVariant Parcel::propertyValue(const QString& key) const
{
    return at(key)->value();
}

QMapIterator<QString, Parcel*> Parcel::propertiesIterator() const
{
    return QMapIterator<QString, Parcel*>(mProperties);
}

Parcel::Type Parcel::type() const
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


