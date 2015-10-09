#include "XmlInputOutput.h"

using namespace blueprint;

XmlInputOutput::XmlInputOutput()
{

}


void blueprint::XmlInputOutput::write(QIODevice& output, const blueprint::SerializeInfo& serializeInfo)
{
    QXmlStreamWriter stream(&output);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    XmlInputOutput::write(stream, serializeInfo);

    stream.writeEndDocument();
}

void XmlInputOutput::write(QXmlStreamWriter& stream, const SerializeInfo& serializeInfo)
{
    // write properties
    auto i = serializeInfo.iterator();
    stream.writeStartElement(serializeInfo.name());
    while (i.hasNext()) {
        i.next();
        stream.writeTextElement(i.key(), i.value().toString());
    }

    // write children
    for(auto child : serializeInfo.children()) {
        XmlInputOutput::write(stream, *child);
    }

    stream.writeEndElement();
}
