#ifndef TESTSERIALIZATION_H
#define TESTSERIALIZATION_H

#include <QTest>

#include "model/Blueprint.h"

namespace blueprint
{

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
    Blueprint mBlueprint;

};
}

#endif // TESTSERIALIZATION_H
