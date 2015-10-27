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

    inline QString name() { return mName; }
    inline void setName(const QString& name) { mName = name; }
    inline Page* page(const int& index) { return mPages[index]; }
    inline int pageCount() const { return mPages.length(); }
    void addPage(Page* page);

    inline Page* activePage() const { return mActivePage; }
    inline void setActivePage(Page* page) { mActivePage = page; }

    Parcel* toParcel() const override;
    void fromParcel(const Parcel& parcel) override;

private:
    QString mName;
    Page* mActivePage;
    QList<Page*> mPages;
};

}

#endif // BLUEPRINT_H
