#ifndef TESTSERIALIZATION_H
#define TESTSERIALIZATION_H

#include <QTest>

#include "model/Blueprint.h"

namespace blueprint
{

class Shape;

class TestSerialization : public QObject
{
    Q_OBJECT
public:
    explicit TestSerialization(QObject *parent = 0);
    ~TestSerialization();

private slots:
    void initTestCase();
    void testSerialization();
    void testDeserialization();

private:
    void compareShapes(const Shape& lhs, const Shape& rhs);
    void compareShapesBezier(const ShapeBezier& lhs, const ShapeBezier& rhs);
private:
    Blueprint mBlueprint;

};
}

#endif // TESTSERIALIZATION_H
