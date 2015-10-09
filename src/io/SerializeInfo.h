#ifndef SERIALIZEINFO_H
#define SERIALIZEINFO_H

#include <QMap>
#include <QString>
#include <QVariant>

namespace blueprint
{

class SerializeInfo
{
public:
    explicit SerializeInfo(const QString& name);

    void addValue(const QString& key, const QVariant& value);
    QVariant value(const QString& key) const;

    QMapIterator<QString, QVariant> iterator() const;

    inline QString name() const { return mName; }

private:
    QString mName;
    QMap<QString, QVariant> mData;
};
}

#endif // SERIALIZEINFO_H
