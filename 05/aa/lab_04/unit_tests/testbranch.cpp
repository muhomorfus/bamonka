#include <QVector3D>
#include <QtTest>

#include "testbranch.h"
#include "inc/branch.h"

TestBranch::TestBranch(QObject *parent)
    : QObject{parent}
{

}

void TestBranch::test_grow_usual()
{
    auto b = new Branch(QVector3D(0, 0, 0), 0.5, 0.5, 100.0, Qt::black, Qt::black);

    b->grow(10);

    QCOMPARE(b->childA == NULL, true);
    QCOMPARE(b->childB == NULL, true);
    QCOMPARE(b->len > 0, true);
}

void TestBranch::test_grow_zero()
{
    auto b = new Branch(QVector3D(0, 0, 0), 0.5, 0.5, 100.0, Qt::black, Qt::black);

    b->grow(0);

    QCOMPARE(b->childA == NULL, true);
    QCOMPARE(b->childB == NULL, true);
    QCOMPARE(qFuzzyIsNull(b->len), true);
}

void TestBranch::test_grow_many()
{
    auto b = new Branch(QVector3D(0, 0, 0), 0.5, 0.5, 10.0, Qt::black, Qt::black);

    b->grow(1000);
    b->grow(1000);

    QCOMPARE(b->childA != NULL, true);
    QCOMPARE(b->childB != NULL, true);
    QCOMPARE(b->len > 0, true);
}

void TestBranch::test_toPolygons_one()
{
    auto b = new Branch(QVector3D(0, 0, 0), 0.5, 0.5, 100.0, Qt::black, Qt::black);
    auto ps = b->toPolygons();

    b->grow(10000);

    QCOMPARE(ps.size(), RING_POLYGONS * 2);
}
