#include <QMainWindow>

#include "point.h"

#define PADDING 20

point::point(int px, int py) {
    x = px;
    y = py;
}

point::point(QPoint qtPoint, point topLeft, point bottomRight, int canvasWidth, int canvasHeight) {
    int paddingX = PADDING * (bottomRight.x - topLeft.x) / (canvasWidth);
    int paddingY = PADDING * (bottomRight.y - topLeft.y) / (canvasHeight);

    bottomRight.x += paddingX;
    bottomRight.y += paddingY;
    topLeft.x -= paddingX;
    topLeft.y -= paddingY;

    double kx = (double) (bottomRight.x - topLeft.x) / canvasWidth;
    double bx = topLeft.x;

    double ky = (double) (bottomRight.y - topLeft.y) / canvasHeight;
    double by = topLeft.y;

    x = qtPoint.x() * kx + bx;
    y = qtPoint.y() * ky + by;
}

QPoint point::toQPoint(point topLeft, point bottomRight, int canvasWidth, int canvasHeight) {
    int paddingX = PADDING * (bottomRight.x - topLeft.x) / (canvasWidth);
    int paddingY = PADDING * (bottomRight.y - topLeft.y) / (canvasHeight);

    bottomRight.x += paddingX;
    bottomRight.y += paddingY;
    topLeft.x -= paddingX;
    topLeft.y -= paddingY;

    double kx = (double) canvasWidth / (bottomRight.x - topLeft.x);
    double bx = -kx * topLeft.x;

    double ky = (double) canvasHeight / (bottomRight.y - topLeft.y);
    double by = -ky * topLeft.y;

    int canvX = x * kx + bx;
    int canvY = y * ky + by;

    return QPoint(canvX, canvY);
}

