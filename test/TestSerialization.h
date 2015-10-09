#ifndef TESTSERIALIZATION_H
#define TESTSERIALIZATION_H

#include <QTest>

class TestSerialization : public QObject
{
    Q_OBJECT
public:
    explicit TestSerialization(QObject *parent = 0);

private slots:
    void initTestCase();
    void testSerialization();
};

#endif // TESTSERIALIZATION_H
