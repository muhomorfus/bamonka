#include <QImage>

#include "task.h"
#include "methods.h"

void drawObject(Object object, QPainter &painter) {
    painter.setPen(QPen(object.color, 1));

    if (object.type == CIRCLE) {
        drawCircle(object.circle, object.method, painter);
    } else if (object.type == ELLIPSE) {
        drawEllipse(object.ellipse, object.method, painter);
    } else if (object.type == CIRCLE_SPECTRE) {
        drawCircleSpectre(object.circleSpectre, object.method, painter);
    } else if (object.type == ELLIPSE_SPECTRE) {
        drawEllipseSpectre(object.ellipseSpectre, object.method, painter);
    }
}

QPixmap drawObjects(std::vector<Object> objects, QImage image) {
    QPainter painter(&image);

    for (int i = 0; i < objects.size(); i++) {
        drawObject(objects[i], painter);
    }

    return QPixmap::fromImage(image);
}

void drawCircle(Circle circle, Method method, QPainter &painter) {
    if (method == CANONICAL) {
        canonicalCircle(circle.center, circle.r, painter);
    } else if (method == PARAMETRIC) {
        parametricCircle(circle.center, circle.r, painter);
    } else if (method == BRESENHAM) {
        bresenhamCircle(circle.center, circle.r, painter);
    } else if (method == MIDDLE_POINT) {
        middlePointCircle(circle.center, circle.r, painter);
    }
}

void drawEllipse(Ellipse ellipse, Method method, QPainter &painter) {
    if (method == CANONICAL) {
        canonicalEllipse(ellipse.center, ellipse.ra, ellipse.rb, painter);
    } else if (method == PARAMETRIC) {
        parametricEllipse(ellipse.center, ellipse.ra, ellipse.rb, painter);
    } else if (method == BRESENHAM) {
        bresenhamEllipse(ellipse.center, ellipse.ra, ellipse.rb, painter);
    } else if (method == MIDDLE_POINT) {
        middlePointEllipse(ellipse.center, ellipse.ra, ellipse.rb, painter);
    }
}

void drawCircleSpectre(CircleSpectre circleSpectre, Method method, QPainter &painter) {
    for (int i = 0; i < circleSpectre.n; i++) {
        drawCircle(circleSpectre.circle, method, painter);
        circleSpectre.circle.r += circleSpectre.dR;
    }
}

void drawEllipseSpectre(EllipseSpectre ellipseSpectre, Method method, QPainter &painter) {
    for (int i = 0; i < ellipseSpectre.n; i++) {
        drawEllipse(ellipseSpectre.ellipse, method, painter);
        ellipseSpectre.ellipse.ra += ellipseSpectre.dRa;
        ellipseSpectre.ellipse.rb += ellipseSpectre.dRb;
    }
}

