//
// Created by muhomorfus on 4/11/22.
//

#include <vector>
#include <fstream>

#include "benches.h"

double benchCircle(Circle circle, Method method) {
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    QImage image = QImage(1000, 1000, QImage::Format_RGB32);
    QPainter painter(&image);

    auto end = high_resolution_clock::now();
    auto start = high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++)
        drawCircle(circle, method, painter);

    end = high_resolution_clock::now();

    return (double) duration_cast<microseconds>(end - start).count() / ITERATIONS;
}

double benchEllipse(Ellipse ellipse, Method method) {
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    QImage image = QImage(1000, 1000, QImage::Format_RGB32);
    QPainter painter(&image);

    auto end = high_resolution_clock::now();
    auto start = high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++)
        drawEllipse(ellipse, method, painter);

    end = high_resolution_clock::now();

    return (double) duration_cast<microseconds>(end - start).count() / ITERATIONS;
}

std::vector<XY> benchCircleSpectre(CircleSpectre circleSpectre, Method method) {
    std::vector<XY> values;

    for (int i = 0; i < circleSpectre.n; i++) {
        double benchResult = benchCircle(circleSpectre.circle, method);
        values.push_back({
                             .x = circleSpectre.circle.r,
                             .y = benchResult,
                         });

        circleSpectre.circle.r += circleSpectre.dR;
    }

    return values;
}

std::vector<XY> benchEllipseSpectre(EllipseSpectre ellipseSpectre, Method method) {
    std::vector<XY> values;

    for (int i = 0; i < ellipseSpectre.n; i++) {
        double benchResult = benchEllipse(ellipseSpectre.ellipse, method);
        values.push_back({
                             .x = ellipseSpectre.ellipse.ra,
                             .y = benchResult,
                         });

        ellipseSpectre.ellipse.ra += ellipseSpectre.dRa;
    }

    return values;
}

void benchWriteMethod(Method method, std::ofstream &out) {
    if (method == CANONICAL) {
        out << "Канонический" << "\n";
    } else if (method == PARAMETRIC) {
        out << "Параметрический" << "\n";
    } else if (method == BRESENHAM) {
        out << "Брезенхем" << "\n";
    } else if (method == MIDDLE_POINT) {
        out << "Ср. линия" << "\n";
    }
}

void benchWriteValues(std::vector<XY> values, std::ofstream &out) {
    for (int i = 0; i < values.size(); i++) {
        out << values[i].x << " " << values[i].y << "\n";
    }
}

std::vector<XY> benchObject(Object object, Method method) {
    if (object.type == CIRCLE_SPECTRE)
        return benchCircleSpectre(object.circleSpectre, method);

    return benchEllipseSpectre(object.ellipseSpectre, method);
}

void benchObjectToFile(Object object, Method method, std::ofstream &out) {
    std::vector<XY> values = benchObject(object, method);
    benchWriteMethod(method, out);
    benchWriteValues(values, out);
    out << "---\n";
}

void bench(Object object, char *filename) {
    std::ofstream out(filename);

    benchObjectToFile(object, CANONICAL, out);
    benchObjectToFile(object, PARAMETRIC, out);
    benchObjectToFile(object, BRESENHAM, out);
    benchObjectToFile(object, MIDDLE_POINT, out);

    out.close();
}
