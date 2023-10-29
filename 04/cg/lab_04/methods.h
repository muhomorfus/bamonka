//
// Created by muhomorfus on 4/10/22.
//

#ifndef METHODS_H
#define METHODS_H

#include <QPainter>
#include <QPoint>

void canonicalCircle(QPoint center, int r, QPainter &painter);

void canonicalEllipse(QPoint center, int ra, int rb, QPainter &painter);

void parametricCircle(QPoint center, int r, QPainter &painter);

void parametricEllipse(QPoint center, int ra, int rb, QPainter &painter);

void bresenhamCircle(QPoint center, int r, QPainter &painter);

void bresenhamEllipse(QPoint center, int ra, int rb, QPainter &painter);

void middlePointCircle(QPoint center, int r, QPainter &painter);

void middlePointEllipse(QPoint center, int ra, int rb, QPainter &painter);

#endif //METHODS_H
