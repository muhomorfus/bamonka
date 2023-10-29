//
// Created by muhomorfus on 4/10/22.
//

#ifndef TASK_H
#define TASK_H

#include <vector>

#include <QPoint>
#include <QColor>
#include <QPainter>
#include <QGraphicsView>

enum ObjectType {
    CIRCLE,
    ELLIPSE,
    CIRCLE_SPECTRE,
    ELLIPSE_SPECTRE,
};

enum Method {
    CANONICAL,
    PARAMETRIC,
    BRESENHAM,
    MIDDLE_POINT,
};

struct Circle {
    QPoint center;

    int r;
};

struct Ellipse {
    QPoint center;

    int ra, rb;
};

struct CircleSpectre {
    Circle circle;

    int dR;
    int n;
};

struct EllipseSpectre {
    Ellipse ellipse;

    int dRa, dRb;
    int n;
};

struct Object {
    ObjectType type;
    Method method;

    QColor color;

    Circle circle;
    Ellipse ellipse;
    CircleSpectre circleSpectre;
    EllipseSpectre ellipseSpectre;
};

void drawObject(Object object, QPainter &painter);

QPixmap drawObjects(std::vector<Object> objects, QImage image);

void drawCircle(Circle circle, Method method, QPainter &painter);

void drawEllipse(Ellipse ellipse, Method method, QPainter &painter);

void drawCircleSpectre(CircleSpectre circleSpectre, Method method, QPainter &painter);

void drawEllipseSpectre(EllipseSpectre ellipseSpectre, Method method, QPainter &painter);

#endif //TASK_H
