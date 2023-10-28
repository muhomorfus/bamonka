#include <cmath>
#include <algorithm>
#include <random>

#include <QVector2D>

#include "inc/forest.h"

#define K 1.2
#define GAP 100
#define PLANE_SCALE 2

Forest::Forest()
{

}

Forest::Forest(int n, int seed, QVector2D bl, QVector2D tr, double y, QColor tc, QColor lc, QColor pc, double r, double s, double ss):
    bottomLeft(bl),
    topRight(tr),
    treeColor(tc),
    leafColor(lc),
    planeColor(pc),
    ratio(r),
    spread(s),
    splitSize(ss),
    y(y)
{
    srand(seed);

    if (n > 0) {
        int trX = tr.x() - GAP;
        int trY = tr.y() - GAP;

        int blX = bl.x() + GAP;
        int blY = bl.y() + GAP;

        double shape = abs((trX - blX) * (trY - blY));
        double sOne = K * shape / n;

        double rad = sqrt(sOne / M_PI);

        std::vector<QVector2D> ps(shape);
        int i = 0;

        for (int y = blY; y < trY; y++) {
            for (int x = blX; x < trX; x++) {
                ps[i] = QVector2D(x, y);
                i++;
            }
        }

        std::default_random_engine e(seed);
        std::shuffle(ps.begin(), ps.end(), e);

        int size = ps.size();

        for (int i = 0; i < n && size > 0; i++) {
            auto pos = ps[rand() % size];

            trees.push_back(new Branch(QVector3D(pos.x(), y, pos.y()), ratio, spread, splitSize, treeColor, leafColor));

            for (int j = 0; j < size; j++) {
                if (ps[j].distanceToPoint(pos) <= rad) {
                    std::swap(ps[j], ps[size - 1]);
                    size--;
                }
            }
        }
    }

    bottomRight = QVector2D(topRight.x(), bottomLeft.y());
    topLeft = QVector2D(bottomLeft.x(), topRight.y());
}

void Forest::grow(int feed)
{
    for (auto &t: trees) {
        t->grow(feed);
    }
}

std::vector<Polygon> Forest::toPolygons()
{
    std::vector<Polygon> result;

    for (auto &t: trees) {
        std::vector<Polygon> treeRes = t->toPolygons();

        result.insert(result.end(), treeRes.begin(), treeRes.end());
    }

    QVector3D a1 = QVector3D(bottomLeft.x(), y, bottomLeft.y());
    QVector3D a2 = QVector3D(topLeft.x(), y, topLeft.y());
    QVector3D a3 = QVector3D(topRight.x(), y, topRight.y());
    QVector3D a4 = QVector3D(bottomRight.x(), y, bottomRight.y());

    auto p1 = Polygon(a1, a2, a3, planeColor, Qt::white);
    auto p2 = Polygon(a3, a4, a1, planeColor, Qt::white);

//    p1.enablePlane();
//    p2.enablePlane();

    result.push_back(p1);
    result.push_back(p2);

    return result;
}
