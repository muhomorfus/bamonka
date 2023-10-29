//
// Created by muhomorfus on 4/10/22.
//

#include <cmath>

#include "methods.h"

void mirrorTo4(int x, int y, QPoint center, QPainter &painter) {
    int cx = center.x();
    int cy = center.y();

    painter.drawPoint(x, y);
    painter.drawPoint(2 * cx - x, y);
    painter.drawPoint(x, 2 * cy - y);
    painter.drawPoint(2 * cx - x, 2 * cy - y);
}

void mirrorTo8(int x, int y, QPoint center, QPainter &painter) {
    mirrorTo4(x, y, center, painter);

    int cx = center.x();
    int cy = center.y();

    painter.drawPoint(y + cx - cy, x + cy - cx);
    painter.drawPoint(-y + cx + cy, x + cy - cx);
    painter.drawPoint(y + cx - cy, -x + cy + cx);
    painter.drawPoint(-y + cx + cy, -x + cx + cy);
}

void canonicalCircle(QPoint center, int r, QPainter &painter) {
    for (int x = 0; x <= ceil(r / std::sqrt(2)); x++) {
        int y = std::sqrt(r * r - x * x);
        mirrorTo8(x + center.x(), y + center.y(), center, painter);
    }
}

void canonicalEllipse(QPoint center, int ra, int rb, QPainter &painter) {
    double ra2 = ra * ra;
    double rb2 = rb * rb;

    int limit = center.x() + ra / sqrt(1 + rb2 / ra2);

    for (int x = center.x(); x <= limit; x++) {
        int relX = x - center.x();
        int y = sqrt(ra2 * rb2 - relX * relX * rb2) / ra + center.y();

        mirrorTo4(x, y, center, painter);
    }

    limit = center.y() + rb / sqrt(1 + ra2 / rb2);

    for (int y = limit; y >= center.y(); y--) {
        int relY = y - center.y();
        int x = sqrt(ra2 * rb2 - relY * relY * ra2) / rb + center.x();

        mirrorTo4(x, y, center, painter);
    }
}

void parametricCircle(QPoint center, int r, QPainter &painter) {
    double step = M_PI_4 / r;

    for (double t = 0; t <= M_PI_4; t += step) {
        int x = center.x() + r * cos(t);
        int y = center.y() + r * sin(t);

        mirrorTo8(x, y, center, painter);
    }
}

void parametricEllipse(QPoint center, int ra, int rb, QPainter &painter) {
    double step = M_PI_4 / ra;
    if (rb > ra) {
        step = M_PI_4 / rb;
    }

    for (double t = 0; t <= M_PI_2; t += step) {
        int x = center.x() + ra * cos(t);
        int y = center.y() + rb * sin(t);

        mirrorTo4(x, y, center, painter);
    }
}

void bresenhamCircle(QPoint center, int r, QPainter &painter) {
    int x = 0;
    int y = r;

    int delta = 2 * (1 - r);

    mirrorTo8(x + center.x(), y + center.y(), center, painter);

    while (x < y) {
        if (delta < 0) {
            int t = 2 * (delta + y) - 1;
            x++;

            if (t > 0) {
                y--;
                delta += 2 * (x - y + 1);
            } else
                delta += 2 * x + 1;
        } else {
            int t = 2 * (delta - x) - 1;
            y--;

            if (t < 0) {
                x++;
                delta += 2 * (x - y + 1);
            } else
                delta -= 2 * y - 1;
        }

        mirrorTo8(x + center.x(), y + center.y(), center, painter);
    }
}

void bresenhamEllipse(QPoint center, int ra, int rb, QPainter &painter) {
    int x = 0;
    int y = rb;

    int ra2 = ra * ra;
    int rb2 = rb * rb;

    int delta = rb2 - ra2 * (2 * rb - 1);

    while (y >= 0) {
        mirrorTo4(x + center.x(), y + center.y(), center, painter);

        if (delta < 0) {
            int t = 2 * delta + ra2 * (2 * y - 1);
            x++;
            delta += rb2 * (2 * x + 1);

            if (t > 0) {
                y--;
                delta += ra2 * (-2 * y + 1);
            }
        } else if (delta == 0) {
            x++;
            y--;
            delta += rb2 * (2 * x + 1) + (1 - 2 * y) * ra2;
        } else {
            int t = 2 * delta + rb2 * (-2 * x - 1);
            y--;
            delta += ra2 * (-2 * y + 1);

            if (t < 0) {
                x++;
                delta += rb2 * (2 * x + 1);
            }
        }
    }
}

void middlePointCircle(QPoint center, int r, QPainter &painter) {
    int x = 0;
    int y = r;

    int delta = 1 - r;

    while (x <= y) {
        mirrorTo8(x + center.x(), y + center.y(), center, painter);
        x++;

        if (delta >= 0) {
            y--;
            delta -= y * 2;
        }

        delta += x * 2 + 1;
    }
}

void middlePointEllipse(QPoint center, int ra, int rb, QPainter &painter) {
    int x = 0;
    int y = rb;

    int ra2 = ra * ra;
    int rb2 = rb * rb;

    double delta = rb2 - ra2 * rb + 0.25 * ra2;
    double dx = 2 * rb2 * x, dy = 2 * ra2 * y;

    while (dx <= dy) {
        mirrorTo4(x + center.x(), y + center.y(), center, painter);
        x++;
        dx += 2 * rb * rb;

        if (delta >= 0) {
            y--;
            dy -= 2 * ra2;
            delta -= dy;
        }

        delta += dx + rb2;
    }

    delta = double(rb2) * (x + 0.5) * (x + 0.5) + double(ra2) * (y - 1) * (y - 1) - double(ra2) * rb2;

    while (y >= 0) {
        mirrorTo4(x + center.x(), y + center.y(), center, painter);

        y--;
        dy -= 2 * ra2;

        if (delta <= 0) {
            x++;
            dx += 2 * rb2;
            delta += dx;
        }

        delta -= dy - ra2;
    }
}
