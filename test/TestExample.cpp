#include "TestExample.h"

#include <QDebug>
#include <QPointF>

#include "TestUtils.h"
#include "model/ShapeFactory.h"
#include "model/ShapeRectangle.h"
#include "model/Canvas.h"

using namespace blueprint;


TestExample::TestExample(QObject *parent) : QObject(parent)
{

}

void TestExample::initTestCase()
{
    qDebug() << "TestExample::initTestCase()";
    TestUtils::toggleLogOutput(false);
}

void TestExample::cleanupTestCase()
{
    qDebug() << "TestExample::cleanupTestCase()";
}

void TestExample::testInt()
{
    int val = 42;
    QCOMPARE(val, 42);
}

void TestExample::testString()
{
    QString text = "Hello World!";
    QCOMPARE(text, QString("Hello World!"));
}

void TestExample::testShape()
{
    Canvas canvas(nullptr, 0, 0);
    Shape* shape = ShapeFactory::createShape(Shape::ShapeType::RECTANGLE,
                                         canvas,
                                         QPointF(0, 0));
    shape->setName("MyShape");
    QCOMPARE(shape->name(), QString("MyShape"));
}

