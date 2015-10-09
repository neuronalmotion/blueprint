#include "TestSerialization.h"

#include <QFile>

#include "io/SerializeInfo.h"
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
    mBlueprint.addPage(page);

    page = new Page();
    page->setName("Page 2");
    mBlueprint.addPage(page);
}

void TestSerialization::testSerialization()
{
    SerializeInfo* serializeInfo = mBlueprint.serialize();

//    QCOMPARE(serializeInfo->value("name").toString(), shape->name());
//    QCOMPARE(serializeInfo->value("type").toInt(), static_cast<int>(shape->shapeType()));

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
}

