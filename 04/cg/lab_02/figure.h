#ifndef FIGURE_H
#define FIGURE_H

#include <vector>

#include "point.h"

typedef struct {
    point a;
    point b;
} relation;

class figure
{
private:
    std::vector<relation> thinLines;
    std::vector<relation> boldLines;
    std::vector<point> boldPoints;
public:
    figure(std::vector<relation> thinLines, std::vector<relation> boldLines, std::vector<point> boldPoints);
    figure();

    void move(int shiftX, int shiftY);
    void resize(point base, double ratioX, double ratioY);
    void rotate(point base, int angle);

    void revertMove(int shiftX, int shiftY);
    void revertResize(point base, double ratioX, double ratioY);
    void revertRotate(point base, int angle);

    void drop();

    void draw(QPainter *painter, QColor color, point topLeft, point bottomRight, int canvasWidth, int canvasHeight);
};

figure defaultElephant();

#endif // FIGURE_H
