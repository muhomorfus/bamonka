#include <cmath>

#include <QVector3D>

#include "inc/sphereshell.h"
#include "inc/ray.h"

SphereShell::SphereShell()
{

}

SphereShell::SphereShell(QVector3D c, double r): center(c), radius(r) {
    squareRadius = r * r;
}

bool SphereShell::traceRay(const Ray& ray)
{
    QVector3D l = center - ray.camera;
    double tca = QVector3D::dotProduct(l, ray.direction);
    double d2 = QVector3D::dotProduct(l, l) - tca * tca;

    if (d2 > squareRadius)
        return false;

    if (tca < 0 && QVector3D::dotProduct(l, l) < squareRadius)
        return false;

    double thc = sqrt(squareRadius - d2);

    double t0 = tca - thc, t1 = tca + thc;
    if (t0 <= 1.0 && t1 <= 1.0)
        return false;

    return true;
}
