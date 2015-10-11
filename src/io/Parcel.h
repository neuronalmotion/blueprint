#ifndef PARCEL_H
#define PARCEL_H

#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

namespace blueprint
{

class Parcel
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

    explicit Parcel(const QString& name);
    ~Parcel();

    void putProperty(const QString& key, const QVariant& value);
    void putProperty(const QString& key, Parcel* info);
    void addPropertyToKey(const QString& key, Parcel* info);

    Parcel* at(const QString& key) const;
    bool contains(const QString& key) const;
    QVariant propertyValue(const QString& key) const;
    inline QVariant value() const { return mValue; }
    void setValue(const QVariant& value);

    inline void addElement(Parcel* element) { mList.append(element); }
    inline const QList<Parcel*>& list() const { return mList; }

    QMapIterator<QString, Parcel*> propertiesIterator() const;

    Type type() const;
    QString typeToString() const { return Parcel::typeToString(type()); }

    inline QString name() const { return mName; }
    inline void setName(const QString& name) { mName = name; }

private:
    QString mName;
    QVariant mValue;
    QList<Parcel*> mList;
    QMap<QString, Parcel*> mProperties;
};
}

#endif // PARCEL_H
