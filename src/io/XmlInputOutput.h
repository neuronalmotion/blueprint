#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QIODevice>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace blueprint
{

class Parcel;

class XmlInputOutput
{
public:
    explicit XmlInputOutput();
    ~XmlInputOutput();

    Parcel* read(QIODevice& input);
    void write(QIODevice& output, const Parcel& parcel);

private:
    Parcel* read(QXmlStreamReader& stream);
    void readValue(QXmlStreamReader& stream, Parcel& parcel);
    void readList(QXmlStreamReader& stream, Parcel& parcel);
    void readObject(QXmlStreamReader& stream, Parcel& parcel);

    void write(QXmlStreamWriter& stream, const Parcel& parcel);
    void writeValue(QXmlStreamWriter& stream, const Parcel& parcel);
    void writeList(QXmlStreamWriter& stream, const Parcel& parcel);
    void writeObject(QXmlStreamWriter& stream, const Parcel& parcel);
};
}

#endif // XMLWRITER_H
