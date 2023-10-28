#include <cmath>

#include <QMatrix4x4>
#include <QTransform>
#include <QQuaternion>

#include "inc/branch.h"

#define SPLIT_DECAY 0.2
#define NUMERATOR 100
#define DIRECTNESS -0.5
#define LOCAL_DEPTH 2
#define TAPER 0.6
#define BRANCH_SCALE 3

void Branch::grow(double feed) {
    radius = sqrt(area / M_PI);

    if (leaf) {
        double lenAddition = cbrt(feed);
        len += lenAddition;

        feed -= lenAddition * area;

        area += feed / len;

        if (canSplit()) {
            split();
        }
    } else {
        double childArea = childA->area + childB->area;
        double k = childArea / (childArea + area);

        area += k * feed / len;

        feed *= 1.0 - k;

        if (qFuzzyIsNull(k)) {
            return;
        }

        childA->grow(feed * ratio);
        childB->grow(feed * (1.0 - ratio));
    }
}

double randSign() {
    return (rand() % 2) * 2 - 1;
}

QVector3D interpolate(QVector3D a, QVector3D b, double k) {
    return a * (1.0 - k) + b * k;
}

void Branch::split() {
    leaf = false;

    childA = new Branch(this, 0);
    childB = new Branch(this, 1);

    auto d = leafDensity(LOCAL_DEPTH);

    auto n = QVector3D::crossProduct(dir, d);
    n.normalize();

    auto m = -n;

    double flippedSpread = randSign() * spread;

    auto ns = n * flippedSpread;
    auto ms = m * flippedSpread;

    childA->dir = interpolate(ns, dir, ratio).normalized();
    childB->dir = interpolate(ms, dir, 1.0 - ratio).normalized();
}

QVector3D randomQVector3D() {
    return QVector3D(rand() % NUMERATOR, rand() % NUMERATOR, rand() % NUMERATOR) / double(NUMERATOR) - QVector3D(0.5, 0.5, 0.5);
}

QVector3D Branch::leafAverage(Branch *b) {
    if (b->leaf) {
        return b->len * b->dir;
    }

    return b->len * b->dir + b->ratio * leafAverage(b->childA) + (1.0 - b->ratio) * leafAverage(b->childB);
}

QVector3D rotateAboutVector(const QVector3D &v, double a, const QVector3D &axis) {
    QQuaternion q = QQuaternion::rotationTo(axis.normalized(), QVector3D(0, 1, 0));
    QVector3D res = q.rotatedVector(v);

    QMatrix4x4 m;
    m.setToIdentity();
    m.rotate(a, QVector3D(0, 1, 0));

    res = m.mapVector(res);

    q = QQuaternion::rotationTo(QVector3D(0, 1, 0), axis.normalized());

    res = q.rotatedVector(res);

    return res;
}

std::vector<Polygon> Branch::toPolygons() {
    auto end = getEnd();

    auto x = dir + QVector3D(1.0, 1.0, 1.0);
    auto n = QVector3D::crossProduct(dir, x).normalized();

    double a = 180.0 / RING_POLYGONS;

    std::vector<Polygon> res;
    std::vector<QVector3D> positions;

    for (int i = 0; i < RING_POLYGONS; i++) {
        positions.push_back(start + radius * BRANCH_SCALE * n);
        n = rotateAboutVector(n, a, dir);

        positions.push_back(end + radius * BRANCH_SCALE * TAPER * n);
        n = rotateAboutVector(n, a, dir);
    }

    for (int i = 0; i < RING_POLYGONS; i++) {
        QVector3D a1 = positions[i * 2];
        QVector3D a2 = positions[(i * 2 + 2) % (2 * RING_POLYGONS)];
        QVector3D a3 = positions[i * 2 + 1];

        QVector3D b1 = positions[(i * 2 + 2) % (2 * RING_POLYGONS)];
        QVector3D b2 = positions[(i * 2 + 3) % (2 * RING_POLYGONS)];
        QVector3D b3 = positions[i * 2 + 1];

        res.push_back(Polygon(a1, a2, a3, color, Qt::white));
        res.push_back(Polygon(b1, b2, b3, color, Qt::white));
    }

    if (leaf && depth > 2) {
        QVector3D a1 = end + dir * 10;
        QVector3D a2 = end + n * 10;
        QVector3D a3 = end - n * 10;
        res.push_back(Polygon(a1, a2, a3, leafColor, Qt::white));
    }

    if (leaf) {
        return res;
    }

    auto polygonsA = childA->toPolygons();
    auto polygonsB = childB->toPolygons();

    res.insert(res.end(), polygonsA.begin(), polygonsA.end());
    res.insert(res.end(), polygonsB.begin(), polygonsB.end());

    return res;
}

QVector3D Branch::getEnd() {
    return start + len * dir;
}

QVector3D Branch::leafDensity(int sd) {
    auto r = randomQVector3D();

    if (!depth) {
        return r;
    }

    Branch *c = this;
    auto rel = QVector3D(0, 0, 0);

    while (c->depth > 0 && sd >= 0) {
        sd--;
        rel += c->len * c->dir;
        c = c->parent;
    }

    return DIRECTNESS * (leafAverage(c) - rel).normalized() + (1.0 - DIRECTNESS) * r;
}

bool Branch::canSplit() {
    return len > splitSize * exp(-SPLIT_DECAY * depth);
}

bool Branch::isLeaf() {
    return leaf;
}
