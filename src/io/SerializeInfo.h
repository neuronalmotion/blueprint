#ifndef SERIALIZEINFO_H
#define SERIALIZEINFO_H

#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

namespace blueprint
{

class SerializeInfo
{
public:
    explicit SerializeInfo(const QString& name);
    ~SerializeInfo();

    void addValue(const QString& key, const QVariant& value);
    QVariant value(const QString& key) const;

    inline void addChild(SerializeInfo* child) { mChildren.append(child); }
    inline const QList<SerializeInfo*>& children() const { return mChildren; }

    QMapIterator<QString, QVariant> iterator() const;

    inline QString name() const { return mName; }
    inline void setName(const QString& name) { mName = name; }

private:
    QString mName;
    QMap<QString, QVariant> mData;
    QList<SerializeInfo*> mChildren;
};
}

#endif // SERIALIZEINFO_H
