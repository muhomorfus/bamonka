#ifndef TASK_H
#define TASK_H

#include <vector>

#include <QPainter>
#include <QPoint>

enum Method {
    DDA,
    BRESENHAM_INT,
    BRESENHAM_FLOAT,
    BRESENHAM_ANTIALIAS,
    WU,
};

enum Type {
    LINE,
    SPECTRE,
};

struct Line {
    QPoint start;
    QPoint end;

    Method method;
    QColor color;
};

struct Spectre {
    QPoint center;

    double radius;
    double angle;

    Method method;
    QColor color;
};

struct Object {
    Type type;

    Spectre spectre;
    Line line;
};

void drawPoint(QPainter &p, QPoint point, double alpha);

int drawLine(QPainter &p, Line line, bool draw = true);
void drawSpectre(QPainter &p, Spectre spectre, bool draw = true);

void drawObject(QPainter &p, Object object, bool draw = true);
void drawObjects(QPainter &p, std::vector<Object> objects);

int drawDDA(QPainter &p, Line line, bool draw = true);
int drawBresenhamInt(QPainter &p, Line line, bool draw = true);
int drawBresenhemFloat(QPainter &p, Line line, bool draw = true);
int drawBresenhemAntiAlias(QPainter &p, Line line, bool draw = true);
int drawWu(QPainter &p, Line line, bool draw = true);

void checkTime(Spectre spectre, QString name = "../lab_03/time.txt");
void checkSteps(Spectre spectre, QString name = "../lab_03/steps.txt");

#endif // TASK_H
