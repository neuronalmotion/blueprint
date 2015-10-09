#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <QList>

#include "io/Serializable.h"
#include "model/Page.h"

namespace blueprint {

class Blueprint : public Serializable
{
public:
    Blueprint();
    ~Blueprint();

    inline void addPage(Page* page) { mPages.append(page); }
    inline QString name() { return mName; }
    inline void setName(const QString& name) { mName = name; }

    SerializeInfo* serialize() const override;
    void deserialize(const SerializeInfo& serializeInfo) override;

private:
    QString mName;
    QList<Page*> mPages;
};

}

#endif // BLUEPRINT_H
