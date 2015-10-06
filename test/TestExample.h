#ifndef TESTEXAMPLE_H
#define TESTEXAMPLE_H

#include <QTest>


class TestExample : public QObject
{
    Q_OBJECT
public:
    explicit TestExample(QObject *parent = 0);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testInt();
    void testString();
    void testShape();
};

#endif // TESTEXAMPLE_H
