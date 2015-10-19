#ifndef PARCELABLE_H
#define PARCELABLE_H

#include "io/Parcel.h"

namespace blueprint
{

class Parcelable
{
public:
    virtual ~Parcelable() {}
    virtual Parcel* toParcel() const = 0;
    virtual void fromParcel(const Parcel& parcel) = 0;
};
}

#endif // PARCELABLE_H
