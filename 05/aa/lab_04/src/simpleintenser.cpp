#include "inc/simpleintenser.h"
#include "inc/polygon.h"

double SimpleIntenser::getDifferencialIntensivity(Polygon &polygon, QVector3D p, Light l) {
    auto mp = (polygon.getA() + polygon.getB() + polygon.getC()) / 3;

    QVector3D lightDir = (mp - l.getPosition()).normalized();

    double prod = QVector3D::dotProduct(-lightDir, polygon.normal);
    if (prod < 0.0) {
        return 0.0;
    }

    return prod * l.getBrightness() * polygon.differencialK;
}

double SimpleIntenser::getSpectacularIntensivity(Polygon &polygon, QVector3D p, Light l, const Ray &ray) {
    return 0.0;
}
