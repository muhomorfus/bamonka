#include <QTest>

#include "testforest.h"
#include "inc/forest.h"

TestForest::TestForest(QObject *parent)
    : QObject{parent}
{

}

void TestForest::test_Forest_one()
{
    Forest f = Forest(1, 0, QVector2D(0, 0), QVector2D(1000, 1000), 500, Qt::black, Qt::black, Qt::black, 0.5, 0.5, 10);

    QCOMPARE(f.trees.size(), 1);
}

void TestForest::test_Forest_many()
{
    Forest f = Forest(10, 0, QVector2D(0, 0), QVector2D(1000, 1000), 500, Qt::black, Qt::black, Qt::black, 0.5, 0.5, 10);

    QCOMPARE(f.trees.size(), 10);
}

void TestForest::test_grow_usual()
{
    Forest f = Forest(5, 0, QVector2D(0, 0), QVector2D(1000, 1000), 500, Qt::black, Qt::black, Qt::black, 0.5, 0.5, 1000);

    f.grow(100);

    for (auto t: f.trees) {
        QCOMPARE(t->len > 0, true);
    }
}

void TestForest::test_toPolygons_usual()
{
    Forest f = Forest(5, 0, QVector2D(0, 0), QVector2D(1000, 1000), 500, Qt::black, Qt::black, Qt::black, 0.5, 0.5, 10);

    f.grow(10);

    auto pols = f.toPolygons();

    QCOMPARE(pols.size(), 5 * 2 * RING_POLYGONS + 2);
}
