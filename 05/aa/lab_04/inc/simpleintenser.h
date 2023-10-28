/**
 * @file
 * @brief Заголовочный файл для класса, который ищет интенсивности света (простая закраска)
 **/

#ifndef SIMPLEINTENSER_H
#define SIMPLEINTENSER_H

#include "inc/intenser.h"

/**
 * @class
 * @brief Класс, который ищет интенсивность (простая закраска)
 */
class SimpleIntenser: public Intenser
{
public:
    /**
     * @brief Конструктор по умолчанию
     */
    SimpleIntenser() = default;

    /**
     * @brief Поиск диффузной составляющей интенсивности
     * @param [in] polygon Полигон, для которого ищется интенсивность
     * @param [in] p Точка, для которой ищется интенсивность
     * @param [in] l Источник света
     * @return Величина интенсивности
     */
    virtual double getDifferencialIntensivity(Polygon &polygon, QVector3D p, Light l);

    /**
     * @brief Поиск глянцевой составляющей интенсивности
     * @param [in] polygon Полигон, для которого ищется интенсивность
     * @param [in] p Точка, для которой ищется интенсивность
     * @param [in] l Источник света
     * @param [in] ray Луч взгляда
     * @return Величина интенсивности
     */
    virtual double getSpectacularIntensivity(Polygon &polygon, QVector3D p, Light l, const Ray &ray);
};

#endif // SIMPLEINTENSER_H
