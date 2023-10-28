/**
 * @file
 * @brief Заголовочный файл для класса, который ищет интенсивности света
 **/

#ifndef INTENSER_H
#define INTENSER_H

#include <vector>

#include <QVector3D>

#include "inc/light.h"
#include "inc/ray.h"

class Polygon;
struct TransformData;

/**
 * @class
 * @brief Класс, который ищет интенсивность (Стратегия)
 */
class Intenser
{
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Intenser();

    /**
     * @brief Поиск диффузной составляющей интенсивности
     * @param [in] polygon Полигон, для которого ищется интенсивность
     * @param [in] p Точка, для которой ищется интенсивность
     * @param [in] l Источник света
     * @return Величина интенсивности
     */
    virtual double getDifferencialIntensivity(Polygon &polygon, QVector3D p, Light l) = 0;

    /**
     * @brief Поиск глянцевой составляющей интенсивности
     * @param [in] polygon Полигон, для которого ищется интенсивность
     * @param [in] p Точка, для которой ищется интенсивность
     * @param [in] l Источник света
     * @param [in] ray Луч взгляда
     * @return Величина интенсивности
     */
    virtual double getSpectacularIntensivity(Polygon &polygon, QVector3D p, Light l, const Ray &ray) = 0;
};

#endif // INTENSER_H
