#include <QtTest>

#include "testpolygon.h"
#include "inc/polygon.h"
#include "inc/ray.h"

TestPolygon::TestPolygon(QObject *parent)
    : QObject{parent}
{

}

void TestPolygon::test_traceRay_usual() {
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    Ray ray = Ray(QVector3D(0, 0, 100), QVector3D(0, 0, -1));
    auto resp = p.traceRay(ray);

    QCOMPARE(resp.ok, true);
    QCOMPARE(qFuzzyCompare(resp.t, 100), true);
}

void TestPolygon::test_traceRay_miss()
{
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    Ray ray = Ray(QVector3D(0, 0, 100), QVector3D(0, 0, 1));
    auto resp = p.traceRay(ray);

    QCOMPARE(resp.ok, false);
}

void TestPolygon::test_getA_usual()
{
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    QCOMPARE(p.pointA, p.getA());
}

void TestPolygon::test_getB_usual()
{
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    QCOMPARE(p.pointB, p.getB());
}

void TestPolygon::test_getC_usual()
{
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    QCOMPARE(p.pointC, p.getC());
}

void TestPolygon::test_setA_usual()
{
    QVector3D v = QVector3D(1, 2, 3);

    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    p.setA(v);

    QCOMPARE(p.pointA, v);
}

void TestPolygon::test_setB_usual()
{
    QVector3D v = QVector3D(1, 2, 3);

    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    p.setB(v);

    QCOMPARE(p.pointB, v);
}

void TestPolygon::test_setC_usual()
{
    QVector3D v = QVector3D(1, 2, 3);

    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    p.setC(v);

    QCOMPARE(p.pointC, v);
}

void TestPolygon::test_getNormal_usual()
{
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::black, Qt::black);

    QCOMPARE(p.getNormal(), QVector3D(0, 0, 1));
}

void TestPolygon::test_computeColor_usual()
{
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::yellow, Qt::green);

    QCOMPARE(p.computeColor(1, 0), Qt::yellow);
    QCOMPARE(p.computeColor(0, 1), Qt::green);
}

void TestPolygon::test_computeColor_zero()
{
    auto p = Polygon(QVector3D(0, 0, 0), QVector3D(100, 0, 0), QVector3D(0, 100, 0), Qt::yellow, Qt::green);

    QCOMPARE(p.computeColor(0, 0), Qt::black);
}
