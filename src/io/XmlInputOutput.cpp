#include "XmlInputOutput.h"

#include <QXmlStreamWriter>

using namespace blueprint;

XmlInputOutput::XmlInputOutput()
{

}


void blueprint::XmlInputOutput::write(QIODevice& output, const blueprint::SerializeInfo& serializeInfo)
{
    QXmlStreamWriter stream(&output);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    auto i = serializeInfo.iterator();
    stream.writeStartElement(serializeInfo.name());
    while (i.hasNext()) {
        i.next();
        stream.writeTextElement(i.key(), i.value().toString());
    }
    stream.writeEndElement();
    stream.writeEndDocument();
}
