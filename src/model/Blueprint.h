#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <QList>

#include "io/Parcelable.h"
#include "model/Page.h"

namespace blueprint {

class Blueprint : public Parcelable
{
public:
    Blueprint();
    ~Blueprint();

    inline void addPage(Page* page) { mPages.append(page); }
    inline QString name() { return mName; }
    inline void setName(const QString& name) { mName = name; }

    Parcel* toParcel() const override;
    void fromParcel(const Parcel& parcel) override;

private:
    QString mName;
    QList<Page*> mPages;
};

}

#endif // BLUEPRINT_H
