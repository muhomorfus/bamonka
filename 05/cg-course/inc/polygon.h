/**
 * @file
 * @brief Заголовочный файл для класса Polygon
 **/

#ifndef POLYGON_H
#define POLYGON_H

#include <cstdlib>
#include <memory>

#include <QColor>
#include <QVector3D>

#include "inc/light.h"
#include "inc/phongintenser.h"
#include "inc/simpleintenser.h"
#include "inc/ray.h"
#include "inc/sphereshell.h"
#include "unit_tests/testpolygon.h"

/**
 * @brief Класс информации о трассировке луча
 */
struct TraceRayData {
    QVector3D point;
    QVector3D n;
    double t;
    bool ok;

    TraceRayData(): ok(false) {}
    TraceRayData(QVector3D point, QVector3D n, double t): point(point), n(n), t(t), ok(true) {}
};

/**
 * @brief Класс полигона
 */
class Polygon
{
public:
    friend class PhongIntenser;
    friend class SimpleIntenser;
    friend class TestPolygon;

    /**
     * @brief Конструктор по умолчанию
     */
    Polygon();

    /**
     * @brief Стандартный конструктор для полигона
     * @param [in] pa Точка A
     * @param [in] pb Точка B
     * @param [in] pc Точка C
     * @param [in] dc Матовый цвет
     * @param [in] sc Глянцевый цвет
     */
    Polygon(QVector3D pa, QVector3D pb, QVector3D pc, QColor dc, QColor sc);

    /**
     * @brief Конструктор копирования
     */
    Polygon(const Polygon &p);

    /**
     * @brief Конструктор переноса
     */
    Polygon(Polygon &&p);

    /**
     * @brief Оператор присваивания через копирование
     */
    Polygon &operator=(const Polygon &p);

    /**
     * @brief Оператор присваивания через перенос
     */
    Polygon &operator=(Polygon &&p);

    /**
     * @brief Получение точки A
     * @return Точка A
     */
    QVector3D getA();

    /**
     * @brief Получение точки B
     * @return Точка B
     */
    QVector3D getB();

    /**
     * @brief Получение точки C
     * @return Точка C
     */
    QVector3D getC();

    /**
     * @brief Установление точки A
     * @param [in] p Точка, которая будет установлена в A
     */
    void setA(QVector3D p);

    /**
     * @brief Установление точки B
     * @param [in] p Точка, которая будет установлена в B
     */
    void setB(QVector3D p);

    /**
     * @brief Установление точки C
     * @param [in] p Точка, которая будет установлена в C
     */
    void setC(QVector3D p);

    /**
     * @brief Получить нормаль к полигону
     * @return Нормально к полигону
     */
    QVector3D getNormal();

    /**
     * @brief Получить матовый цвет
     * @return Матовый цвет
     */
    QColor getDifferencialColor();

    /**
     * @brief Получить глянцевый цвет
     * @return Глянцевый цвет
     */
    QColor getSpectacularColor();

    /**
     * @brief Функция трассировки луча
     * @param [in] ray Луч
     * @return Информация о пересечении полигона лучем
     */
    TraceRayData traceRay(const Ray &ray);

    /**
     * @brief Получить ID полигона
     * @return ID полигона
     */
    int getID();

    /**
     * @brief Вычислить итоговый цвет полигона по заданным интенсивностям
     * @param [in] d Матовая интенсивность
     * @param [in] s Глянцевая интенсивность
     * @return
     */
    QColor computeColor(double d, double s);

private:
    QVector3D pointA, pointB, pointC;

    QColor differencialColor;
    QColor spectacularColor;

    double a, b, c, d;

    QVector3D normal;

    QVector3D camera;

    double differencialK = 0.8;
    double spectacularK = 0.6;

    double spectacularExp = 50.0;

    QVector3D basePoint(Light l);

    void countCoefficients();

    QVector3D ab, bc, ca;

    SphereShell shell;

    int id;

    static int currentID;

    bool plane = false;

    bool inside(const QVector3D &p);
};

#endif // POLYGON_H
