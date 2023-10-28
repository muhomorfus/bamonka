#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light
{
public:
    Light();
    Light(QVector3D p, double b);

    QVector3D getPosition();
    double getBrightness();

private:
    QVector3D position;
    double brightness;
};

#endif // LIGHT_H
