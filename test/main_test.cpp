#include <QTest>
#include <QDebug>

#include "TestExample.h"

int main(int argc, char *argv[])
{
    TestExample testExample;
    return QTest::qExec(&testExample, argc, argv);
}
