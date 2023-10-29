#ifndef POINT_H
#define POINT_H

#include <QMainWindow>

class point
{
public:
    point(int px, int py);
    point(QPoint qtPoint, point topLeft, point bottomRight, int canvasWidth, int canvasHeight);

    QPoint toQPoint(point topLeft, point bottomRight, int canvasWidth, int canvasHeight);

    int x;
    int y;
};

#endif // POINT_H
