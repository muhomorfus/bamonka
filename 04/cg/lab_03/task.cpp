#include <vector>
#include <cmath>
#include <fstream>

#include <QPainter>
#include <QMessageBox>

#include "task.h"

void drawPoint(QPainter &p, QPoint point, double alpha = 1.0) {
    QColor color = p.pen().color();

    int rgbAlpha = alpha * 255;
    if (rgbAlpha > 255)
        rgbAlpha = 255;

    if (rgbAlpha < 0)
        rgbAlpha = 0;

    color.setAlpha(rgbAlpha);

    p.setPen(color);
    p.setBrush(color);
    p.drawRect(point.x(), point.y(), 1, 1);
}

int drawLine(QPainter &p, Line line, bool draw) {
    if (draw) {
        p.setPen(line.color);
        p.setBrush(line.color);
    }

    switch (line.method) {
    case DDA:
        return drawDDA(p, line, draw);
    case BRESENHAM_INT:
        return drawBresenhamInt(p, line, draw);
    case BRESENHAM_FLOAT:
        return drawBresenhemFloat(p, line, draw);
    case BRESENHAM_ANTIALIAS:
        return drawBresenhemAntiAlias(p, line, draw);
    case WU:
        return drawWu(p, line, draw);
    default:
        return 0;
    }
}

double toRadiants(double x) {
    return x * M_PI / 180.0;
}

void drawSpectre(QPainter &p, Spectre spectre, bool draw) {
    if (draw) {
        p.setPen(spectre.color);
        p.setBrush(spectre.color);
    }

    for (double angle = 0.0; angle <= 360.0; angle += spectre.angle) {
        double cx = spectre.center.x();
        double cy = spectre.center.y();

        double x = cx + spectre.radius * cos(toRadiants(angle));
        double y = cy + spectre.radius * sin(toRadiants(angle));

        Line line = { QPoint(cx, cy), QPoint(x, y), spectre.method, spectre.color };
        drawLine(p, line, draw);
    }
}

void drawObject(QPainter &p, Object object, bool draw) {
    switch (object.type) {
    case LINE:
        drawLine(p, object.line, draw);
        return;
    case SPECTRE:
        drawSpectre(p, object.spectre, draw);
        return;
    }
}

void drawObjects(QPainter &p, std::vector<Object> objects) {
    for (int i = 0; i < objects.size(); i++)
        drawObject(p, objects[i]);
}

bool absEquals(double a, double b) {
    return std::round(a) != std::round(b);
}

int drawDDA(QPainter &p, Line line, bool draw) {
    double dx = line.end.x() - line.start.x();
    double dy = line.end.y() - line.start.y();

    double maxD = std::max(std::abs(dx), std::abs(dy));
    double signX = dx / maxD;
    double signY= dy / maxD;

    double x = line.start.x();
    double y = line.start.y();

    double bufX = x;
    double bufY = y;

    int steps = 1;

    for (int i = 0; i < maxD; i++) {
        if (draw)
            drawPoint(p, QPoint(std::round(x), std::round(y)));

        x += signX;
        y += signY;

        if (x != bufX || y != bufY)
            steps++;

        bufX = x;
        bufY = y;
    }

    return steps;
}

double sign(double x) {
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;

    return 0;
}

int drawBresenhamInt(QPainter &p, Line line, bool draw) {
    double dx = line.end.x() - line.start.x();
    double dy = line.end.y() - line.start.y();

    double signX = sign(dx);
    double signY = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

    bool changed = false;
    if (dy >= dx) {
        std::swap(dx, dy);
        changed = true;
    }

    int m = 2 * dy;
    int e = m - dx;

    int x = round(line.start.x());
    int y = round(line.start.y());

    int bufX = x;
    int bufY = y;
    int steps = 1;

    for (int i = 0; i <= dx; i++) {
        if (draw)
            drawPoint(p, QPoint(x, y));

        if (e >= 0) {
            if (changed)
                x += signX;
            else
                y += signY;
            e -= 2 * dx;
        }

        if (e <= 0) {
            if (changed)
                y += signY;
            else
                x += signX;
            e += 2 * dy;
        }

        if (bufX != x && bufY != y)
            steps++;

        bufX = x;
        bufY = y;
    }

    return steps;
}

int drawBresenhemFloat(QPainter &p, Line line, bool draw) {
    double dx = line.end.x() - line.start.x();
    double dy = line.end.y() - line.start.y();

    int signX = sign(dx);
    int signY = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

    bool changed = false;
    if (dy >= dx) {
        std::swap(dx, dy);
        changed = true;
    }

    double m = dy / dx;
    double e = m - 0.5;

    double x = round(line.start.x());
    double y = round(line.start.y());

    double bufX = x;
    double bufY = y;

    int steps = 1;

    for (int i = 0; i <= dx; i++) {
        if (draw)
            drawPoint(p, QPoint(x, y));

        if (e >= 0) {
            if (changed)
                x += signX;
            else
                y += signY;
            e--;
        }
        if (e <= 0) {
            if (changed)
                y += signY;
            else
                x += signX;
            e += m;
        }

        if (bufX != x && bufY != y)
            steps++;

        bufX = x;
        bufY = y;
    }

    return steps;
}

int drawBresenhemAntiAlias(QPainter &p, Line line, bool draw) {
    double dx = line.end.x() - line.start.x();
    double dy = line.end.y() - line.start.y();

    int signX = sign(dx);
    int signY = sign(dy);

    dx = std::abs(dx), dy = std::abs(dy);

    bool changed = false;
    if (dy >= dx) {
        std::swap(dx, dy);
        changed = true;
    }

    double m = dy / dx;
    double e = 0.5;

    double x = round(line.start.x());
    double y = round(line.start.y());

    if (draw)
        drawPoint(p, QPoint(x, y), e);

    double W = 1 - m;

    double bufX = x;
    double bufY = y;

    int steps = 1;

    for (int i = 0; i <= dx; i++) {
        if (e < W) {
            if (!changed)
                x += signX;
            else
                y += signY;
            e += m;
        } else {
            x += signX;
            y += signY;
            e -= W;
        }

        if (draw)
            drawPoint(p, QPoint(x, y), e);

        if (bufX != x && bufY != y)
            steps++;

        bufX = x;
        bufY = y;
    }

    return steps;
}

double fPart(double x) {
    return x - int(x);
}

double iPart(double x) {
    return x - fPart(x);
}

double rfPart(double x) {
    return 1 - fPart(x);
}

double round(double x) {
    return iPart(x + 0.5);
}

int drawWu(QPainter &p, Line line, bool draw) {
    QPoint start = line.start;
    QPoint end = line.end;

    bool steep = false;

    if (std::abs(end.y() - start.y()) > std::abs(end.x() - start.x())) {
        start = QPoint(start.y(), start.x());
        end = QPoint(end.y(), end.x());
        steep = true;
    }

    if (start.x() > end.x()) {
        std::swap(start, end);
    }

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();
    double gradient = (dx == 0) ? 1 : dy / dx;

    int xpx11;
    double interY;

    {
        double xEnd = round(start.x());
        double yEnd = start.y() + gradient * (xEnd - start.x());
        double xGap = rfPart(start.x() + 0.5);
        xpx11 = int(xEnd);
        int ypx11 = iPart(yEnd);

        double alpha1 = rfPart(yEnd) * xGap;
        double alpha2 = fPart(yEnd) * xGap;

        if (draw) {
            if (steep) {
                drawPoint(p, QPoint(ypx11, xpx11), alpha1);
                drawPoint(p, QPoint(ypx11 + 1, xpx11), alpha2);
            } else {
                drawPoint(p, QPoint(xpx11, ypx11), alpha1);
                drawPoint(p, QPoint(xpx11, ypx11 + 1), alpha2);
            }
        }

        interY = yEnd + gradient;
    }

    int xpx12;

    {
        double xEnd = std::round(end.x());
        double yEnd = end.y() + gradient * (xEnd - end.x());
        double xGap = rfPart(end.x() + 0.5);
        xpx12 = int(xEnd);
        int ypx12 = iPart(yEnd);

        double alpha1 = rfPart(yEnd) * xGap;
        double alpha2 = fPart(yEnd) * xGap;
        if (draw) {
            if (steep) {
                drawPoint(p, QPoint(ypx12, xpx12), alpha1);
                drawPoint(p, QPoint(ypx12 + 1, xpx12), alpha2);
            } else {
                drawPoint(p, QPoint(xpx12, ypx12), alpha1);
                drawPoint(p, QPoint(xpx12, ypx12 + 1), alpha2);
            }
        }
    }

    int steps = 0;

    for (int x = xpx11 + 1; x < xpx12; x++) {
        double alpha1 = rfPart(interY);
        double alpha2 = fPart(interY);

        if (draw) {
            if (steep) {
                drawPoint(p, QPoint(iPart(interY), x), alpha1);
                drawPoint(p, QPoint(iPart(interY) + 1, x), alpha2);
            } else {
                drawPoint(p, QPoint(x, iPart(interY)), alpha1);
                drawPoint(p, QPoint(x, iPart(interY) + 1), alpha2);
            }
        }

        if (x < xpx12) {
            if (iPart(interY) != iPart(interY + gradient))
                steps++;
        }

        interY += gradient;
    }

    return steps;
}

double checkDrawSpectreTime(Spectre spectre) {
    int checks = 100;

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    QPainter p;

    auto end = high_resolution_clock::now();
    auto start = high_resolution_clock::now();

    for (int i = 0; i < checks; i++) {
        for (double angle = 0.0; angle <= 360.0; angle += spectre.angle) {
            double cx = spectre.center.x();
            double cy = spectre.center.y();

            double x = cx + spectre.radius * cos(toRadiants(angle));
            double y = cy + spectre.radius * sin(toRadiants(angle));

            Line line = { QPoint(cx, cy), QPoint(x, y), spectre.method, spectre.color };
            drawLine(p, line, false);
        }
    }

    end = high_resolution_clock::now();
    return (double) duration_cast<microseconds>(end - start).count() / checks;
}

void checkTime(Spectre spectre, QString name) {
    std::ofstream out(name.toStdString());
    if (!out.is_open()) {
        QMessageBox::warning(NULL, "Ошибка", "Ошибка открытия файла.");
        return;
    }

    double time;

    spectre.method = DDA;
    out << "ЦДА" << "\n";
    out << checkDrawSpectreTime(spectre) << "\n";

    spectre.method = BRESENHAM_INT;
    out << "Брез. цел." << "\n";
    out << checkDrawSpectreTime(spectre) << "\n";

    spectre.method = BRESENHAM_FLOAT;
    out << "Брез. вещ." << "\n";
    out << checkDrawSpectreTime(spectre) << "\n";

    spectre.method = BRESENHAM_ANTIALIAS;
    out << "Брез. сглаж." << "\n";
    out << checkDrawSpectreTime(spectre) << "\n";

    spectre.method = WU;
    out << "Ву" << "\n";
    out << checkDrawSpectreTime(spectre) << "\n";

    out.close();

    system("python3 ../lab_03/time.py");
}


void writeSpectreStepsInfo(Spectre spectre, std::ofstream &out) {
    QPainter p;

    for (double angle = 0.0; angle <= 360.0; angle += spectre.angle) {
        double cx = spectre.center.x();
        double cy = spectre.center.y();

        double x = cx + spectre.radius * cos(toRadiants(angle));
        double y = cy + spectre.radius * sin(toRadiants(angle));

        Line line = { QPoint(cx, cy), QPoint(x, y), spectre.method };
        int steps = drawLine(p, line, false);

        out << angle << " " << steps << "\n";
    }
}

void checkSteps(Spectre spectre, QString name) {
    std::ofstream out(name.toStdString());
    if (!out.is_open()) {
        QMessageBox::warning(NULL, "Ошибка", "Ошибка открытия файла.");
        return;
    }

    double time;

    spectre.method = DDA;
    out << "ЦДА" << "\n";
    writeSpectreStepsInfo(spectre, out);
    out << "---\n";

    spectre.method = BRESENHAM_INT;
    out << "Брез. цел." << "\n";
    writeSpectreStepsInfo(spectre, out);
    out << "---\n";

    spectre.method = BRESENHAM_FLOAT;
    out << "Брез. вещ." << "\n";
    writeSpectreStepsInfo(spectre, out);
    out << "---\n";

    spectre.method = BRESENHAM_ANTIALIAS;
    out << "Брез. сглаж." << "\n";
    writeSpectreStepsInfo(spectre, out);
    out << "---\n";

    spectre.method = WU;
    out << "Ву" << "\n";
    writeSpectreStepsInfo(spectre, out);

    out.close();

    system("python3 ../lab_03/steps.py");
}
