/**
 * @file
 * @brief Заголовочный файл класса Branch
 */

#ifndef BRANCH_H
#define BRANCH_H

#include <memory>

#include <QVector3D>

#include "inc/polygon.h"
#include "unit_tests/testbranch.h"
#include "unit_tests/testforest.h"

/**
 * @def RING_POLYGONS Количество рядов полигонов в одной ветке
 */
#define RING_POLYGONS 6

/**
 * @class
 * @brief Ветка дерева
 */
class Branch
{
    friend class TestBranch;
    friend class TestForest;

public:
    /**
     * @brief Конструктор по умолчанию
     */
    Branch() {};

    /**
     * @param [in] start Точка начала ветки
     * @param [in] r Соотношение сторон потомков-веток
     * @param [in] s Коэффициент густоты кроны
     * @param [in] ss Длина ветки, после которой она раздвоится
     * @param [in] c Цвет кроны
     * @param [in] lc Цвет листьев
     * @brief Конструктор корневой ветки
     */
    Branch(QVector3D start, double r, double s, double ss, QColor c, QColor lc):
        start(start),
        ratio(r),
        spread(s),
        splitSize(ss),
        color(c),
        leafColor(lc) {}

    /**
     * @param [in] p Указатель на ветку-родителя
     * @param [in] n Число, которое влияет на ID ветки. Для одного потомка это будет 1, для другого 2
     * @brief Конструктор веток-потомков
     */
    Branch(Branch *p, int n): Branch(p->getEnd(), p->ratio, p->spread, p->splitSize, p->color, p->leafColor) {
        parent = p;
        depth = p->depth + 1;
        id = p->id * 2 + n;
    }

    /**
     * @param [in] b Указатель на ветку
     * @brief Конструктор, которые копирует значение из указателя
     */
    Branch(Branch *b):
        id(b->id),
        leaf(b->leaf),
        parent(b->parent),
        childA(b->childA),
        childB(b->childB),
        start(b->start),
        ratio(b->ratio),
        spread(b->spread),
        splitSize(b->splitSize),
        depth(b->depth),
        len(b->len),
        radius(b->radius),
        area(b->area),
        dir(b->dir) {}

    /**
     * @brief Деструктор
     */
    ~Branch() {
        delete childA;
        delete childB;
    }

    /**
     * @param [in] feed Количество питательных веществ, поставляемых в ветку
     * @brief "Подкормить" ветку
     */
    void grow(double feed);

    /**
     * @brief Преобразовать ветку в полигоны
     * @return Массив полигонов ветки
     */
    std::vector<Polygon> toPolygons();

private:
    int id = 1;
    bool leaf = true;

    Branch *parent;

    Branch *childA;
    Branch *childB;

    QVector3D start;

    double ratio;
    double spread;
    double splitSize;

    int depth = 0;

    double len = 0.0;
    double radius = 0.0;
    double area = 0.1;

    QVector3D dir = QVector3D(0, -1.0, 0);

    QColor color;
    QColor leafColor;

    QVector3D getEnd();

    void split();

    QVector3D leafDensity(int sd);

    bool canSplit();
    bool isLeaf();

    static QVector3D leafAverage(Branch *b);
};

#endif // BRANCH_H
