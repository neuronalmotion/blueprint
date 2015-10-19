#include "XmlInputOutput.h"

#include "Parcel.h"

using namespace blueprint;

XmlInputOutput::XmlInputOutput()
{

}

XmlInputOutput::~XmlInputOutput()
{

}

Parcel* XmlInputOutput::read(QIODevice& input)
{
    QXmlStreamReader stream(&input);
    stream.readNextStartElement();
    return XmlInputOutput::read(stream);
}

Parcel* XmlInputOutput::read(QXmlStreamReader& stream)
{
    Parcel* parcel = new Parcel(stream.name().toString());
    QXmlStreamAttributes attributes = stream.attributes();
    Parcel::Type type = Parcel::stringToType(attributes.value("type").toString());
    switch (type) {
    case Parcel::Type::VALUE:
        readValue(stream, *parcel);
    break;

    case Parcel::Type::LIST:
        readList(stream, *parcel);
    break;

    case Parcel::Type::OBJECT:
        readObject(stream, *parcel);
    break;

    default:
    break;
    }
    return parcel;
}

void XmlInputOutput::readValue(QXmlStreamReader& stream, Parcel& parcel)
{
    parcel.setValue(stream.readElementText());
}

void XmlInputOutput::readList(QXmlStreamReader& stream, Parcel& parcel)
{
    while (stream.readNextStartElement()) {
        parcel.addElement(read(stream));
    }
}

void XmlInputOutput::readObject(QXmlStreamReader& stream, Parcel& parcel)
{
    while (stream.readNextStartElement()) {
        auto child = read(stream);
        parcel.putProperty(child->name(), child);
    }
}

void blueprint::XmlInputOutput::write(QIODevice& output, const blueprint::Parcel& parcel)
{
    QXmlStreamWriter stream(&output);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    XmlInputOutput::write(stream, parcel);

    stream.writeEndDocument();
}

void XmlInputOutput::write(QXmlStreamWriter& stream, const Parcel& parcel)
{
    switch (parcel.type()) {
    case Parcel::Type::VALUE:
        writeValue(stream, parcel);
    break;

    case Parcel::Type::LIST:
        writeList(stream, parcel);
    break;

    case Parcel::Type::OBJECT: {
        writeObject(stream, parcel);
    }
    break;
    default:
        qFatal("Undefined Parcel type");
    break;
    }
}

void XmlInputOutput::writeValue(QXmlStreamWriter& stream, const Parcel& parcel)
{
    stream.writeTextElement(parcel.name(), parcel.value().toString());
}

void XmlInputOutput::writeList(QXmlStreamWriter& stream, const Parcel& parcel)
{
    for (auto child : parcel.list()) {
        XmlInputOutput::write(stream, *child);
    }
}

void XmlInputOutput::writeObject(QXmlStreamWriter& stream, const Parcel& parcel)
{
    stream.writeStartElement(parcel.name());
    stream.writeAttribute("type", parcel.typeToString());
    auto i = parcel.propertiesIterator();
    while (i.hasNext()) {
        i.next();
        Parcel* property = i.value();
        Parcel::Type type = property->type();
        if (type == Parcel::Type::LIST) {
            stream.writeStartElement(i.key());
            stream.writeAttribute("type", property->typeToString());
        }
        XmlInputOutput::write(stream, *property);
        if (type == Parcel::Type::LIST) {
            stream.writeEndElement();
        }
    }
    stream.writeEndElement();
}
