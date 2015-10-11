#include "TestSerialization.h"

#include <QFile>

#include "io/Parcel.h"
#include "io/XmlInputOutput.h"

#include "TestUtils.h"
#include "model/Canvas.h"
#include "model/Page.h"
#include "model/Shape.h"
#include "model/ShapeFactory.h"
#include "model/ShapeRectangle.h"

using namespace blueprint;

TestSerialization::TestSerialization(QObject *parent)
    : QObject(parent),
      mBlueprint()
{

}

TestSerialization::~TestSerialization()
{
}

void TestSerialization::initTestCase()
{
    TestUtils::toggleLogOutput(false);

    mBlueprint.setName("Test Project");

    Page* page = new Page();
    page->setName("Page 1");

    page->appendChild(ShapeFactory::createShape(Shape::ShapeType::CANVAS, *page));
    page->appendChild(ShapeFactory::createShape(Shape::ShapeType::CANVAS, *page));

    Shape* canvas = page->child(0);
    Shape* rectangle = ShapeFactory::createShape(Shape::ShapeType::RECTANGLE, *canvas);
    canvas->appendChild(rectangle);
    mBlueprint.addPage(page);

    page = new Page();
    page->setName("Page 2");
    mBlueprint.addPage(page);
}

void TestSerialization::testSerialization()
{
    Parcel* parcel = mBlueprint.toParcel();

//    QCOMPARE(parcel->value("name").toString(), shape->name());
//    QCOMPARE(parcel->value("type").toInt(), static_cast<int>(shape->shapeType()));

    QFile output("test.xml");
    output.open(QIODevice::ReadWrite);
    XmlInputOutput xml;
    xml.write(output, *parcel);

    output.seek(0);
    QString result(output.readAll());

    TestUtils::toggleLogOutput(true);
    qDebug() << "Result" << result;
    TestUtils::toggleLogOutput(false);

    output.close();
    delete parcel;
}

void TestSerialization::testDeserialization()
{
    QFile input("test.xml");
    input.open(QIODevice::ReadOnly);
    XmlInputOutput xml;
    Parcel* parcel = xml.read(input);
    input.close();

    Blueprint blueprint;
    blueprint.fromParcel(*parcel);
    QCOMPARE(blueprint.name(), mBlueprint.name());

    delete parcel;
}

