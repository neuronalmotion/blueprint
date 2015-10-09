#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QIODevice>
#include <QXmlStreamWriter>

#include "SerializeInfo.h"

namespace blueprint
{

class XmlInputOutput
{
public:
    XmlInputOutput();

    static void write(QIODevice& output, const SerializeInfo& serializeInfo);

private:
    static void write(QXmlStreamWriter& stream, const SerializeInfo& serializeInfo);


};
}

#endif // XMLWRITER_H
