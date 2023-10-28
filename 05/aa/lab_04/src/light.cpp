#include "inc/light.h"

Light::Light()
{

}

Light::Light(QVector3D p, double b): position(p), brightness(b) {}

QVector3D Light::getPosition() {
    return position;
}

double Light::getBrightness() {
    return brightness;
}
