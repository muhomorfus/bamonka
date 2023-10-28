#include <cmath>

#include "inc/phongintenser.h"
#include "inc/polygon.h"

double PhongIntenser::getDifferencialIntensivity(Polygon &polygon, QVector3D p, Light l) {
    QVector3D lightDir = (p - l.getPosition()).normalized();

    double prod = QVector3D::dotProduct(-lightDir, polygon.normal);
    if (prod < 0.0) {
        return 0.0;
    }

    return prod * l.getBrightness() * polygon.differencialK;
}

double PhongIntenser::getSpectacularIntensivity(Polygon &polygon, QVector3D p, Light l, const Ray &ray) {
    QVector3D lightDir = (p - l.getPosition()).normalized();

    QVector3D reflectedLightDir = lightDir - 2.0 * QVector3D::dotProduct(polygon.normal, lightDir) * polygon.normal;
    reflectedLightDir.normalize();

    QVector3D watchDir = ray.direction.normalized();
    watchDir = QVector3D(0, 0, -1);

    double prod = QVector3D::dotProduct(reflectedLightDir, -watchDir);

    if (prod < 0.0) {
        return 0.0;
    }

    return pow(prod, polygon.spectacularExp) * l.getBrightness() * polygon.spectacularK;
}

