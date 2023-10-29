#ifndef POINT_H
#define POINT_H

#include <QMainWindow>

class point
{
public:
    point(int px, int py);
    point(QPoint qtPoint, point topLeft, point bottomRight, int canvasWidth, int canvasHeight);

    QPoint toQPoint(point topLeft, point bottomRight, int canvasWidth, int canvasHeight);

    void move(int shiftX, int shiftY);
    void resize(point base, double ratioX, double ratioY);
    void rotate(point base, int angle);

    int x;
    int y;
};

#endif // POINT_H
