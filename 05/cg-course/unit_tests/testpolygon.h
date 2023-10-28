#ifndef TESTPOLYGON_H
#define TESTPOLYGON_H

#include <QObject>

class TestPolygon : public QObject
{
    Q_OBJECT
public:
    explicit TestPolygon(QObject *parent = nullptr);

private slots:
    void test_traceRay_usual();
    void test_traceRay_miss();

    void test_getA_usual();
    void test_getB_usual();
    void test_getC_usual();

    void test_setA_usual();
    void test_setB_usual();
    void test_setC_usual();

    void test_getNormal_usual();

    void test_computeColor_usual();
    void test_computeColor_zero();

signals:

};

#endif // TESTPOLYGON_H
