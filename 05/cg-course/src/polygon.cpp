#include <cmath>

#include "inc/polygon.h"

int Polygon::currentID = 0;

Polygon::Polygon() {}

Polygon::Polygon(QVector3D pa, QVector3D pb, QVector3D pc, QColor dc, QColor sc): pointA(pa), pointB(pb), pointC(pc), differencialColor(dc), spectacularColor(sc) {
    ab = pointB - pointA;
    bc = pointC - pointB;
    ca = pointA - pointC;

    id = currentID + 1;
    currentID++;

    countCoefficients();
}

void Polygon::countCoefficients() {
    QVector3D watchDirection = QVector3D(0, 0, -1);

    double mx = (pointB.y() - pointA.y()) * (pointC.z() - pointA.z()) - (pointC.y() - pointA.y()) * (pointB.z() - pointA.z());
    double my = (pointB.x() - pointA.x()) * (pointC.z() - pointA.z()) - (pointC.x() - pointA.x()) * (pointB.z() - pointA.z());
    double mz = (pointB.x() - pointA.x()) * (pointC.y() - pointA.y()) - (pointC.x() - pointA.x()) * (pointB.y() - pointA.y());

    a = mx;
    b = -my;
    c = mz;
    d = -mx * pointA.x() + my * pointA.y() - mz * pointA.z();

    QVector3D n = QVector3D(a, b, c);

    if (QVector3D::dotProduct(watchDirection, n) >= 0) {
        a *= -1;
        b *= -1;
        c *= -1;
        d *= -1;
    }

    normal = QVector3D(a, b, c).normalized();

    QVector3D center = (pointA + pointB + pointC) / 3;
    double ra = center.distanceToPoint(pointA);
    double rb = center.distanceToPoint(pointB);
    double rc = center.distanceToPoint(pointC);

    shell = SphereShell(center, std::max({ra, rb, rc}));
}

Polygon::Polygon(const Polygon &p) {
    *this = p;
}

Polygon::Polygon(Polygon &&p) {
    *this = p;
}

Polygon &Polygon::operator=(const Polygon &p) {
    pointA = p.pointA;
    pointB = p.pointB;
    pointC = p.pointC;

    differencialColor = p.differencialColor;
    spectacularColor = p.spectacularColor;

    a = p.a;
    b = p.b;
    c = p.c;
    d = p.d;

    normal = p.normal;

    ab = p.ab;
    bc = p.bc;
    ca = p.ca;

    shell = p.shell;

    id = p.id;

    plane = p.plane;

    return *this;
}

Polygon &Polygon::operator=(Polygon &&p) {
    pointA = p.pointA;
    pointB = p.pointB;
    pointC = p.pointC;

    differencialColor = p.differencialColor;
    spectacularColor = p.spectacularColor;

    a = p.a;
    b = p.b;
    c = p.c;
    d = p.d;

    normal = p.normal;

    ab = p.ab;
    bc = p.bc;
    ca = p.ca;

    shell = p.shell;

    id = p.id;

    plane = p.plane;

    return *this;
}

QVector3D Polygon::getA() {
    return pointA;
}

QVector3D Polygon::getB() {
    return pointB;
}

QVector3D Polygon::getC() {
    return pointC;
}

void Polygon::setA(QVector3D p) {
    pointA = p;
    countCoefficients();
}

void Polygon::setB(QVector3D p) {
    pointB = p;
    countCoefficients();
}

void Polygon::setC(QVector3D p) {
    pointC = p;
    countCoefficients();
}

QVector3D Polygon::getNormal() {
    return normal;
}

QColor Polygon::getDifferencialColor() {
    return differencialColor;
}

QColor Polygon::getSpectacularColor() {
    return spectacularColor;
}

QVector3D rotatePointX(QVector3D p, double angle) {
    angle = angle * M_PI / 180;

    QVector3D res;
    res.setY(p.y() * cos(angle) + p.z() * sin(angle));
    res.setZ(-p.y() * sin(angle) + p.z() * cos(angle));
    res.setX(p.x());

    return res;
}

TraceRayData Polygon::traceRay(const Ray &ray) {
    if (!shell.traceRay(ray)) {
        return TraceRayData();
    }

    QVector3D dir = ray.direction;
    QVector3D start = ray.camera;

    double k = a * dir.x() + b * dir.y() + c * dir.z();
    if (qFuzzyIsNull(k))
        return TraceRayData();

    double t = -(a * start.x() + b * start.y() + c * start.z() + d) / k;
    if (t < 0)
        return TraceRayData();

    QVector3D isc = start + dir * t;
    if (!inside(isc))
        return TraceRayData();

    return TraceRayData(isc, QVector3D(a, b, c).normalized(), t);
}

inline int sign(double a) {
    if (qFuzzyIsNull(a)) {
        return 0;
    }

    if (a > 0) {
        return 1;
    }

    return -1;
}

inline bool equalSign(double v1, double v2) {
    return sign(v1) * sign(v2) >= 0;
}

inline bool equalSigns(const QVector3D &v1, const QVector3D &v2) {
    return equalSign(v1.x(), v2.x()) && equalSign(v1.y(), v2.y()) && equalSign(v1.z(), v2.z());
}

inline bool Polygon::inside(const QVector3D &p) {
    auto ap = p - pointA;
    auto bp = p - pointB;
    auto cp = p - pointC;

    auto m1 = QVector3D::crossProduct(ab, ap);
    auto m2 = QVector3D::crossProduct(bc, bp);
    auto m3 = QVector3D::crossProduct(ca, cp);

    return equalSigns(m1, m2) && equalSigns(m2, m3) && equalSigns(m3, m1);
}

int Polygon::getID() {
    return id;
}

inline int border(int c, int n) {
    if (c > n) {
        return n;
    }

    return c;
}

inline QColor sum(QColor a, QColor b) {
    return QColor(
                border(a.red() + b.red(), 255),
                border(a.green() + b.green(), 255),
                border(a.blue() + b.blue(), 255)
    );
}

inline QColor withIntensivity(QColor c, double i) {
    if (i > 1) {
        i = 1;
    }

    return QColor(c.red() * i, c.green() * i, c.blue() * i);
}

QColor Polygon::computeColor(double d, double s)
{
    if (plane) {
        return differencialColor;
    }

    return sum(withIntensivity(differencialColor, d), withIntensivity(spectacularColor, s));
}


