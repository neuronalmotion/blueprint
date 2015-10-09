#include "TestSerialization.h"

#include <QFile>

#include "io/SerializeInfo.h"
#include "io/XmlInputOutput.h"

#include "TestUtils.h"
#include "model/Canvas.h"
#include "model/Shape.h"
#include "model/ShapeFactory.h"
#include "model/ShapeRectangle.h"

using namespace blueprint;

TestSerialization::TestSerialization(QObject *parent) : QObject(parent)
{

}

void TestSerialization::initTestCase()
{
    TestUtils::toggleLogOutput(false);
}

void TestSerialization::testSerialization()
{
    Canvas canvas(nullptr, 0, 0);
    Shape* shape = ShapeFactory::createShape(Shape::ShapeType::RECTANGLE,
                                         canvas,
                                         QPointF(0, 0));
    shape->setName("MyRectangle");

    SerializeInfo* serializeInfo = shape->serialize();

    QCOMPARE(serializeInfo->value("name").toString(), shape->name());
    QCOMPARE(serializeInfo->value("type").toInt(), static_cast<int>(shape->shapeType()));

    QFile output("test.xml");
    output.open(QIODevice::ReadWrite);
    XmlInputOutput::write(output, *serializeInfo);

    output.seek(0);
    QString result(output.readAll());

    TestUtils::toggleLogOutput(true);
    qDebug() << "Result" << result;
    TestUtils::toggleLogOutput(false);

    output.close();
    delete serializeInfo;
    delete shape;
}

