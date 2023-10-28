/**
 * @file
 * @brief Заголовочный файл для класса, который ищет интенсивности света (метод Фонга)
 **/

#ifndef PHONGINTENSER_H
#define PHONGINTENSER_H

#include "inc/intenser.h"

/**
 * @class
 * @brief Класс, который ищет интенсивность (Фонг)
 */
class PhongIntenser: public Intenser {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    PhongIntenser() = default;

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

#endif // PHONGINTENSER_H
