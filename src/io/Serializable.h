#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "io/SerializeInfo.h"

namespace blueprint
{

class Serializable
{
public:
    virtual ~Serializable() {}
    virtual SerializeInfo* serialize() const = 0;
    virtual void deserialize(const SerializeInfo& serializeInfo) = 0;
};
}

#endif // SERIALIZABLE_H
