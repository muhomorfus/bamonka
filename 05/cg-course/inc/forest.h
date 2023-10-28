/**
 * @file
 * @brief Заголовочный файл класса Forest
 */

#ifndef FOREST_H
#define FOREST_H

#include <QVector2D>

#include "inc/branch.h"
#include "inc/polygon.h"
#include "unit_tests/testforest.h"

/**
 * @class
 * @brief Лес
 */
class Forest
{
    friend class TestForest;

public:
    /**
     * @brief Конструктор по умолчанию
     */
    Forest();

    /**
     * @brief Лес
     * @param [in] n Количество деревьев в лесу
     * @param [in] seed Ключ генерации леса
     * @param [in] bl Минимальные координаты по осям X и Z
     * @param [in] tr Максимальные координаты по осям X и Z
     * @param [in] y Координата Y леса
     * @param [in] tc Цвет крон деревьев
     * @param [in] lc Цвет листьев
     * @param [in] pc Цвет плоскости
     * @param [in] r Показатель ассиметрии деревьев
     * @param [in] s Показатель густоты кроны
     * @param [in] ss Длина, после которой будет разделение веток
     */
    Forest(int n, int seed, QVector2D bl, QVector2D tr, double y, QColor tc, QColor lc, QColor pc, double r, double s, double ss);

    /**
     * @brief "Подкормить" лес, то есть все деревья леса
     * @param [in] feed Величина подкормки
     */
    void grow(int feed);

    /**
     * @brief Преобразование леса в полигоны
     * @return Массив полигонов леса
     */
    std::vector<Polygon> toPolygons();

private:
    std::vector<Branch*> trees;

    QVector2D bottomLeft, topRight;
    QVector2D bottomRight, topLeft;

    QColor treeColor;
    QColor leafColor;
    QColor planeColor;

    double ratio;
    double spread;
    double splitSize;

    double y;

};

#endif // FOREST_H
