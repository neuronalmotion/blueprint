#include <QTest>
#include <QDebug>

#include "TestSerialization.h"

int main(int argc, char *argv[])
{
    TestSerialization testSerialization;
    return QTest::qExec(&testSerialization, argc, argv);
}
