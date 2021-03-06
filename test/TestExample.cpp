#include "TestExample.h"

#include <QDebug>
#include <QPointF>

#include "TestUtils.h"
#include "model/ShapeFactory.h"
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
    Shape* shape = ShapeFactory::createShape(Shape::ShapeType::RECTANGLE);
    shape->setName("MyShape");
    QCOMPARE(shape->name(), QString("MyShape"));
}

