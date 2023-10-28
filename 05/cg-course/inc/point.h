#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point();
    Point(int x, int y, int z);
    Point(const Point &p);

    Point &operator=(const Point &p);
    Point &operator=(Point &&p);

    Point operator+(Point p);

    Point operator/(double n);

    int getX();
    int getY();
    int getZ();

    void setX(int x);
    void setY(int y);
    void setZ(int z);

private:
    int x, y, z;
};

#endif // POINT_H
