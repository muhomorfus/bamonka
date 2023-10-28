#include "point.h"

Point::Point() {}

Point::Point(int x, int y, int z): x(x), y(y), z(z) {}

Point::Point(const Point &p) {
    x = p.x;
    y = p.y;
    z = p.z;
}

Point &Point::operator=(const Point &p) {
    x = p.x;
    y = p.y;
    z = p.z;

    return *this;
}

Point &Point::operator=(Point &&p) {
    x = p.x;
    y = p.y;
    z = p.z;

    return *this;
}

Point Point::operator+(Point p) {
    return Point(x + p.x, y + p.y, z + p.z);
}

Point Point::operator/(double n) {
    return Point(x / n, y / n, z / n);
}

int Point::getX() {
    return x;
}

int Point::getY() {
    return y;
}

int Point::getZ() {
    return z;
}

void Point::setX(int x) {
    this->x = x;
}

void Point::setY(int y) {
    this->y = y;
}

void Point::setZ(int z) {
    this->z = z;
}
