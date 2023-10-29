
#ifndef TASK_H
#define TASK_H

#include <vector>
#include <cmath>

#include <QColor>
#include <QPoint>
#include <QGraphicsView>

typedef enum {
    EVENT_FIGURE,
    EVENT_LINE,
    EVENT_DELETE_FIGURE,
} event_t;

typedef struct
{
    QPoint p1;
    QPoint p2;
} line_t;

typedef struct
{
    bool exists;
    std::vector<QPoint> points;
} figure_t;

typedef struct
{
    QColor cutter_color;
    QColor invisible_color;
    QColor visible_color;
} colors_t;

QPoint get_vector(QPoint p1, QPoint p2);
int vector_prod(QPoint v1, QPoint v2);
int scalar_prod(QPoint p1, QPoint p2);

void get_d_k_b(int ax, int ay, int cx, int cy, double& d, double& k, double& b);
bool are_crossed(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy);
bool check_cross(figure_t cutter);

int is_convex(figure_t cutter);

void parallel_point(int& x, int& y, figure_t cutter, std::vector<line_t> lines);

void draw_all(colors_t colors, bool draw_line, bool progress_line, bool draw_figure, bool progress_figure, std::vector<line_t> lines, figure_t figure, QGraphicsView* view);

int sign(int num);

#endif //TASK_H
