
#ifndef TASK_H
#define TASK_H

#include <cmath>

#include <QGraphicsView>
#include <QImage>
#include <QPoint>

typedef enum {
    EVENT_CUTTER,
    EVENT_FIGURE,
    EVENT_DELETE_CUTTER,
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

bool cut(figure_t& res, figure_t fig, figure_t cutter);

int is_convex(figure_t cutter);

int sign(int num);

void draw_all(colors_t colors, bool draw_figures, bool progress_figures, bool draw_figure, bool progress_figure, std::vector<figure_t> figures, figure_t figure, QGraphicsView* view);

#endif //TASK_H
