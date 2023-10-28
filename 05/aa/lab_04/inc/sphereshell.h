/**
 * @file
 * @brief Заголовочный файл для класса SphereShell
 **/

#ifndef SPHERESHELL_H
#define SPHERESHELL_H

#include "inc/ray.h"

/**
 * @brief Класс сфрической оболочки
 */
class SphereShell
{
public:
    /**
     * @brief Конструктор по умолчанию
     */
    SphereShell();

    /**
     * @brief Конструктор с радиусом и центром
     * @param [in] c Координаты центра
     * @param [in] r Радиус
     */
    SphereShell(QVector3D c, double r);

    /**
     * @brief traceRay
     * @param [in] ray Трассировка луча на сферической оболочке
     * @return Признак, пересекает ли луч оболочку
     */
    bool traceRay(const Ray& ray);

private:
    QVector3D center;
    double radius;
    double squareRadius;
};

#endif // SPHERESHELL_H
