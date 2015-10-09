#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QIODevice>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace blueprint
{

class SerializeInfo;

class XmlInputOutput
{
public:
    explicit XmlInputOutput();
    ~XmlInputOutput();

    SerializeInfo* read(QIODevice& input);
    void write(QIODevice& output, const SerializeInfo& serializeInfo);

private:
    bool isProperty(const QString& tagName);
    SerializeInfo* read(QXmlStreamReader& stream);
    void write(QXmlStreamWriter& stream, const SerializeInfo& serializeInfo);


};
}

#endif // XMLWRITER_H
