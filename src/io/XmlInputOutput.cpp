#include "XmlInputOutput.h"

#include "SerializeInfo.h"

using namespace blueprint;

XmlInputOutput::XmlInputOutput()
{

}

XmlInputOutput::~XmlInputOutput()
{

}

SerializeInfo* XmlInputOutput::read(QIODevice& input)
{
    QXmlStreamReader stream(&input);
    stream.readNextStartElement();
    return XmlInputOutput::read(stream);
}

SerializeInfo* XmlInputOutput::read(QXmlStreamReader& stream)
{
    SerializeInfo* serializeInfo = new SerializeInfo(stream.name().toString());
    while (stream.readNextStartElement()) {
        QString name(stream.name().toString());

        if (isProperty(name)) {
            serializeInfo->addValue(name, stream.readElementText());
        } else {
            serializeInfo->addChild(read(stream));
        }
    }
    return serializeInfo;
}


void blueprint::XmlInputOutput::write(QIODevice& output, const blueprint::SerializeInfo& serializeInfo)
{
    QXmlStreamWriter stream(&output);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    XmlInputOutput::write(stream, serializeInfo);

    stream.writeEndDocument();
}

bool XmlInputOutput::isProperty(const QString& tagName)
{
    return ! (tagName == "blueprint"
                | tagName == "page"
                | tagName == "canvas"
    );
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
