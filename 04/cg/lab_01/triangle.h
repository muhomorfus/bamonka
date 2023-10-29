#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"

class triangle
{
public:
    triangle(point a, point b, point c);
    triangle();
    void copy(triangle &dst);

    bool isPointInside(point p);
    int numOfPointsInside(std::vector<point> ps);

    int numOfPointsOutsideSide(std::vector<point> ps, int i, int j);

    point points[3] = { point(0, 0), point(0, 0), point(0, 0) };

    bool empty;
};

triangle findSolution(std::vector<point> points);

#endif // TRIANGLE_H
