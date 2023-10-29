#include <vector>
#include <math.h>

#include "triangle.h"
#include "point.h"

int sign(int x) {
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;
    return 0;
}

triangle::triangle() {
    empty = true;
}

triangle::triangle(point a, point b, point c) {
    points[0] = a;
    points[1] = b;
    points[2] = c;
    empty = false;
}

void triangle::copy(triangle &dst) {
    for (int i = 0; i < 3; i++)
        dst.points[i] = points[i];
    dst.empty = empty;
}

bool triangle::isPointInside(point p) {
    int abX = points[1].x - points[0].x;
    int abY = points[1].y - points[0].y;

    int bcX = points[2].x - points[1].x;
    int bcY = points[2].y - points[1].y;

    int caX = points[0].x - points[2].x;
    int caY = points[0].y - points[2].y;

    int apX = p.x - points[0].x;
    int apY = p.y - points[0].y;

    int bpX = p.x - points[1].x;
    int bpY = p.y - points[1].y;

    int cpX = p.x - points[2].x;
    int cpY = p.y - points[2].y;

    int mul1 = abX * apY - abY * apX;
    int mul2 = bcX * bpY - bcY * bpX;
    int mul3 = caX * cpY - caY * cpX;

    return (mul1 < 0 && mul2 < 0 && mul3 < 0) || (mul1 > 0 && mul2 > 0 && mul3 > 0);
}

int triangle::numOfPointsInside(std::vector<point> ps) {
    int counter = 0;

    for (int i = 0; i < ps.size(); i++)
        if (isPointInside(ps[i]))
            counter++;

    return counter;
}

int triangle::numOfPointsOutsideSide(std::vector<point> ps, int i, int j) {
    int k = 3 - i - j;
    int a = points[j].y - points[i].y;
    int b = points[i].x - points[j].x;
    int c = points[i].y * points[j].x - points[i].x * points[j].y;

    int sideSign = sign(a * points[k].x + b * points[k].y + c);
    int counter = 0;
    for (int l = 0; l < ps.size(); l++) {
        int curSign = sign(a * ps[l].x + b * ps[l].y + c);
        if (curSign != sideSign && curSign != 0)
            counter++;
    }

    return counter;
}

bool isValid(std::vector<point> points, triangle current) {
    int counterAB = current.numOfPointsOutsideSide(points, 0, 1);
    int counterBC = current.numOfPointsOutsideSide(points, 1, 2);
    int counterCA = current.numOfPointsOutsideSide(points, 2, 0);

    return counterAB == counterBC && counterBC == counterCA;
}

bool isBetter(std::vector<point> points, triangle current, triangle best) {
    if (best.empty)
        return true;

    return current.numOfPointsInside(points) > best.numOfPointsInside(points);
}

triangle findSolution(std::vector<point> points) {
    triangle best = triangle();

    for (int i = 0; i < points.size(); i++)
        for (int j = i + 1; j < points.size(); j++)
            for (int k = j + 1; k < points.size(); k++)
            {
                triangle current = triangle(points[i], points[j], points[k]);
                if (isValid(points, current))
                    if (isBetter(points, current, best))
                        current.copy(best);
            }

    return best;
}
