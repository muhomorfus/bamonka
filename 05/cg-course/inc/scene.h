/**
 * @file
 * @brief Заголовочный файл для класса Scene
 **/

#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include <QImage>

#include "inc/polygon.h"
#include "inc/light.h"
#include "inc/ray.h"
#include "inc/forest.h"

/**
 * @brief Класс с информацией об интенсивностях
 */
struct Intensivities {
    double spectacular;
    double diffuse;
};

/**
 * @brief Класс с информацией о цвете и его наличии
 */
struct ColorData {
    bool haveColor;
    QColor color;

    /**
     * @brief Конструктор по умолчанию
     */
    ColorData(): haveColor(false) {}

    /**
     * @brief Конструктор по цвету
     * @param [in] c Цвет
     */
    ColorData(QColor c): haveColor(true), color(c) {}
};

/**
 * @brief Класс сцены
 */
class Scene
{
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Scene();

    /**
     * @brief Конструктор с размерами
     * @param [in] w Ширина
     * @param [in] h Высота
     */
    Scene(int w, int h);

    /**
     * @brief Конструктор с размерами и полигонами
     * @param [in] w Ширина
     * @param [in] h Высота
     * @param [in] ps Список полигонов
     */
    Scene(int w, int h, std::vector<Polygon> ps);

    /**
     * @brief Конструктор с полигонами
     * @param [in] ps Список полигонов
     */
    Scene(std::vector<Polygon> ps);

    /**
     * @brief Установить размер сцены
     * @param [in] w Ширина
     * @param [in] h Высота
     */
    void setSize(int w, int h);

    /**
     * @brief Установка камеры
     * @param [in] c Координаты камеры
     */
    void setCamera(QVector3D c);

    /**
     * @brief Отрисовать сцену
     * @param [in] nthreads Количество потоков
     * @return Изображение
     */
    std::shared_ptr<QImage> paint(int nthreads = 0);

    /**
     * @brief Трассировка луча
     * @param [in] ray Луч
     * @return
     */
    QColor traceRay(const Ray &ray);

    /**
     * @brief Установить лес
     * @param [in] f Лес
     */
    void setForest(Forest &f);

    /**
     * @brief Установить цвет фона
     * @param [in] color Цвет фона
     */
    void setBackground(QColor color);

    /**
     * @brief "Подкормить лес"
     * @param [in] feed Количество питательных веществ
     * @param [in] n Количество итераций подкормки
     */
    void growForest(double feed, int n);

private:
    int width = 1200, height = 800;

    std::vector<Polygon> polygons;

    std::vector<Light> lights;

    Forest forest;

    QVector3D camera = QVector3D(0, 0, 2000);

    QColor background = QColor(229,234,206);

    std::shared_ptr<Intenser> intenser = std::make_shared<PhongIntenser>();

    ColorData getColor(const QVector3D &p, const Ray &ray, std::vector<Light> &lights, Polygon &polygon);
    bool isVisible(const Ray &ray, Polygon &polygon);

    Intensivities getIntensivity(const QVector3D &p, Light &l, Polygon &polygon, const Ray &ray);
    Intensivities getIntensivityMany(const QVector3D &p, std::vector<Light> &ls, Polygon &polygon, const Ray &ray);

    void paintThread(std::vector<std::vector<QColor>> &cbuf, int start, int size);
};

#endif // SCENE_H
