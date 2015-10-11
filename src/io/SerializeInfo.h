#ifndef SERIALIZEINFO_H
#define SERIALIZEINFO_H

#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

namespace blueprint
{

const QString IO_NAME_BLUEPRINT = QString("blueprint");
const QString IO_NAME_PAGE = QString("page");
const QString IO_NAME_CANVAS = QString("canvas");
const QString IO_NAME_SHAPE = QString("shape");

class SerializeInfo
{
public:

    enum Type {
        UNDEFINED,
        VALUE,
        LIST,
        OBJECT,
    };

    static Type stringToType(const QString& string);
    static QString typeToString(const Type& type);

    explicit SerializeInfo(const QString& name);
    ~SerializeInfo();

    void putProperty(const QString& key, const QVariant& value);
    void putProperty(const QString& key, SerializeInfo* info);
    void addPropertyToKey(const QString& key, SerializeInfo* info);

    SerializeInfo* at(const QString& key) const;
    QVariant propertyValue(const QString& key) const;
    inline QVariant value() const { return mValue; }
    void setValue(const QVariant& value);

    inline void addElement(SerializeInfo* element) { mList.append(element); }
    inline const QList<SerializeInfo*>& list() const { return mList; }

    QMapIterator<QString, SerializeInfo*> propertiesIterator() const;

    Type type() const;
    QString typeToString() const { return SerializeInfo::typeToString(type()); }

    inline QString name() const { return mName; }
    inline void setName(const QString& name) { mName = name; }

private:
    QString mName;
    QVariant mValue;
    QList<SerializeInfo*> mList;

    QMap<QString, SerializeInfo*> mProperties;
};
}

#endif // SERIALIZEINFO_H
