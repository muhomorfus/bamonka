#include <memory>
#include <limits>
#include <cmath>
#include <iostream>
#include <string>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <QPainter>
#include <QDebug>

#include "inc/scene.h"
#include "inc/branch.h"
#include "inc/forest.h"

#define SIMPLE "simple"
#define PHONG "phong"

Scene::Scene() {
    lights = {
        Light(QVector3D(100, 000, 500), 0.9),
    };

    camera = QVector3D(300, 300, 900);
}

Scene::Scene(int w, int h): Scene() {
    width = w;
    height = h;
}

void Scene::setSize(int w, int h) {
    width = w;
    height = h;
}

void Scene::setCamera(QVector3D c) {
    camera = c;
}

QColor Scene::traceRay(const Ray &ray) {
    TraceRayData data;
    size_t index = 0;

    bool first = true;

    for (size_t i = 0; i < polygons.size(); i++) {
        auto res = polygons[i].traceRay(ray);

        if (!res.ok) {
            continue;
        }

        if (first || res.t < data.t) {
            index = i;
            data = res;
            first = false;
        }
    }

    if (first) {
        return background;
    }

    auto col = getColor(data.point, ray, lights, polygons[index]);
    if (!col.haveColor) {
        return Qt::black;
    }

    return col.color;
}

void Scene::setForest(Forest &f)
{
    forest = f;
}

void Scene::setBackground(QColor color)
{
    background = color;
}

void Scene::growForest(double feed, int n)
{
    for (int i = 0; i < n; i++) {
        forest.grow(feed);
    }
}

Scene::Scene(int w, int h, std::vector<Polygon> ps): width(w), height(h), polygons(ps) {}

Scene::Scene(std::vector<Polygon> ps): polygons(ps) {}

std::shared_ptr<QImage> Scene::paint(int nthreads) {
    polygons = forest.toPolygons();

    auto image = std::make_shared<QImage>(width, height, QImage::Format_RGB32);
    image->fill(Qt::black);

    QPainter painter(image.get());

    std::vector<std::vector<QColor>> colorBuffer(height, std::vector<QColor>(width, Qt::black));

    if (nthreads == 0) {
        Ray ray = Ray(camera, QVector3D());

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                ray.direction = (QVector3D(x, y, 0) - camera).normalized();

                colorBuffer[y][x] = traceRay(ray);
            }
        }
    } else {
        int hw = height * width;

        int n =  hw / nthreads;
        if (hw % nthreads != 0) {
            n++;
        }

        int start = 0;

        std::vector<std::thread> threads(nthreads);
        for (int i = 0; i < nthreads; i++) {
            threads[i] = std::thread(&Scene::paintThread, this, std::ref(colorBuffer), start, n);
            start += n;
        }

        for (int i = 0; i < nthreads; i++)
            threads[i].join();
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            painter.setPen(colorBuffer[y][x]);
            painter.drawPoint(x, y);
        }
    }

    return image;
}

Intensivities Scene::getIntensivity(const QVector3D &p, Light &l, Polygon &polygon, const Ray &ray) {
    auto lpos = l.getPosition();

    Intensivities result;
    result.diffuse = 0.0;
    result.spectacular = 0.0;

    Ray lray = Ray(lpos, p - lpos);

    if (!isVisible(lray, polygon)) {
        return result;
    }

    result.diffuse = intenser->getDifferencialIntensivity(polygon, p, l);
    result.spectacular = intenser->getSpectacularIntensivity(polygon, p, l, ray);

    return result;
}

Intensivities Scene::getIntensivityMany(const QVector3D &p, std::vector<Light> &ls, Polygon &polygon, const Ray &ray) {
    Intensivities result;
    result.diffuse = 0.0;
    result.spectacular = 0.0;

    for (std::size_t i = 0; i < ls.size(); i++) {
        auto ins = getIntensivity(p, ls[i], polygon, ray);
        result.diffuse += ins.diffuse;
        result.spectacular += ins.spectacular;
    }

    return result;
}

void Scene::paintThread(std::vector<std::vector<QColor>> &cbuf, int start, int size)
{
    int maxSize = width * height;
    int end = start + size;
    if (end > maxSize) {
        end = maxSize;
    }

    for (int n = start; n < end; n++) {
        int y = n / width;
        int x = n % width;

        Ray ray = Ray(camera, (QVector3D(x, y, 0) - camera).normalized());

        cbuf[y][x] = traceRay(ray);
    }
}

ColorData Scene::getColor(const QVector3D &p, const Ray &ray, std::vector<Light> &lights, Polygon &polygon) {
    auto ins = getIntensivityMany(p, lights, polygon, ray);

    if (qFuzzyIsNull(ins.diffuse) && qFuzzyIsNull(ins.spectacular)) {
        return ColorData();
    }

    return ColorData(polygon.computeColor(ins.diffuse, ins.spectacular));
}

bool Scene::isVisible(const Ray &ray, Polygon &polygon) {
    auto resp = polygon.traceRay(ray);
    if (!resp.ok) {
        return false;
    }

    auto id = polygon.getID();

    for (auto p: polygons) {
        if (p.getID() == id) {
            continue;
        }

        auto resp1 = p.traceRay(ray);
        if (!resp1.ok) {
            continue;
        }

        if (resp1.t >= 0 && resp1.t < resp.t) {
            return false;
        }
    }

    return true;
}
