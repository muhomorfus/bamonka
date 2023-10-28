/**
 * @file
 * @brief Заголовочный файл для класса Ray
 **/
#ifndef RAY_H
#define RAY_H

#include <QVector3D>

/**
 * @brief Класс луча
 */
struct Ray {
    /**
     * @brief Камера
     */
    QVector3D camera;

    /**
     * @brief Направление луча
     */
    QVector3D direction;

    /**
     * @brief Конструктор луча
     * @param [in] c Координаты камеры
     * @param [in] d Направление луча
     */
    Ray(QVector3D c, QVector3D d): camera(c), direction(d) {}
};

#endif // RAY_H
