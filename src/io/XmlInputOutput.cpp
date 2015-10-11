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
    QXmlStreamAttributes attributes = stream.attributes();
    SerializeInfo::Type type = SerializeInfo::stringToType(attributes.value("type").toString());
    switch (type) {
    case SerializeInfo::Type::VALUE:
        serializeInfo->setValue(stream.readElementText());
    break;
    case SerializeInfo::Type::LIST:
        while (stream.readNextStartElement()) {
            serializeInfo->addElement(read(stream));
        }
    break;
    case SerializeInfo::Type::OBJECT:
        while (stream.readNextStartElement()) {
            auto child = read(stream);
            serializeInfo->putProperty(child->name(), child);
        }
    break;
    default:
    break;
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

void XmlInputOutput::write(QXmlStreamWriter& stream, const SerializeInfo& serializeInfo)
{
    switch (serializeInfo.type()) {
    case SerializeInfo::Type::VALUE:
        stream.writeTextElement(serializeInfo.name(), serializeInfo.value().toString());

    break;
    case SerializeInfo::Type::LIST:
        for (auto child : serializeInfo.list()) {
            XmlInputOutput::write(stream, *child);
        }

    break;
    case SerializeInfo::Type::OBJECT: {
        stream.writeStartElement(serializeInfo.name());
        stream.writeAttribute("type", serializeInfo.typeToString());
        auto i = serializeInfo.propertiesIterator();
        while (i.hasNext()) {
            i.next();
            SerializeInfo* property = i.value();
            SerializeInfo::Type type = property->type();
            if (type == SerializeInfo::Type::LIST) {
                stream.writeStartElement(i.key());
                stream.writeAttribute("type", property->typeToString());
            }
            XmlInputOutput::write(stream, *property);
            if (type == SerializeInfo::Type::LIST) {
                stream.writeEndElement();
            }
        }
        stream.writeEndElement();
    }
    break;
    default:
        qFatal("Undefined Parcel type");
    break;
    }
}
