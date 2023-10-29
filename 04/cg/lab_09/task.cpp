
#include "task.h"

#define EPS 1

QPoint get_vector(QPoint p1, QPoint p2)
{
    return QPoint(p2.x() - p1.x(), p2.y() - p1.y());
}

int vector_prod(QPoint v1, QPoint v2)
{
    return v1.x() * v2.y() - v1.y() * v2.x();
}

int sign(int num)
{
    if (num == 0)
        return 0;
    return (num > 0) ? 1 : -1;
}

bool is_visible(line_t line, QPoint p, int norm)
{
    QPoint v1 = line.p2 - line.p1;
    QPoint v2 = p - line.p2;

    int pr = v1.x() * v2.y() - v1.y() * v2.x();

    if (pr * norm >= 0)
        return true;

    return false;
}

bool find_intersection(QPoint& p, line_t line, line_t sec, int norm)
{
    bool vis1 = is_visible(line, sec.p1, norm);
    bool vis2 = is_visible(line, sec.p2, norm);
    bool is_inter = (vis1 || vis2) && (!(vis1 && vis2));

    if (is_inter) {
        QPoint cut_vec = line.p2 - line.p1;
        QPoint pol_vec = sec.p2 - sec.p1;

        long nominator = cut_vec.y() * (sec.p1.x() - line.p1.x()) - cut_vec.x() * (sec.p1.y() - line.p1.y());
        long denominator = pol_vec.y() * cut_vec.x() - pol_vec.x() * cut_vec.y();

        if (denominator == 0) {
            p = sec.p2;
        } else {
            double t = (double)nominator / denominator;
            p = sec.p1 + (sec.p2 - sec.p1) * t;
        }
    }

    return is_inter;
}

int is_convex(figure_t cutter)
{
    if (cutter.points.size() < 3)
        return 1;

    QPoint p1, p2, p3;
    int x1, y1, x2, y2, r, prev = 0, curr;
    for (size_t i = 0; i < cutter.points.size() - 2; i++) {
        p1 = cutter.points[i];
        p2 = cutter.points[i + 1];
        p3 = cutter.points[i + 2];

        x1 = p2.x() - p1.x();
        y1 = p2.y() - p1.y();

        x2 = p3.x() - p2.x();
        y2 = p3.y() - p2.y();

        r = x1 * y2 - x2 * y1;
        curr = sign(r);

        if (!prev)
            prev = curr;
        if (curr && curr != prev)
            return 0;
    }

    p1 = cutter.points[cutter.points.size() - 2];
    p2 = cutter.points[cutter.points.size() - 1];
    p3 = cutter.points[0];

    x1 = p2.x() - p1.x();
    y1 = p2.y() - p1.y();

    x2 = p3.x() - p2.x();
    y2 = p3.y() - p2.y();

    r = x1 * y2 - x2 * y1;
    curr = sign(r);
    if (curr && curr != prev)
        return 0;

    p1 = cutter.points[cutter.points.size() - 1];
    p2 = cutter.points[0];
    p3 = cutter.points[1];

    x1 = p2.x() - p1.x();
    y1 = p2.y() - p1.y();

    x2 = p3.x() - p2.x();
    y2 = p3.y() - p2.y();

    r = x1 * y2 - x2 * y1;
    curr = sign(r);
    if (curr && curr != prev)
        return 0;

    QPoint tmp;
    for (size_t i = 0; i < cutter.points.size() - 1; i++) {
        line_t edge1 = {
            .p1 = cutter.points[i],
            .p2 = cutter.points[(i + 1) % cutter.points.size()],
        };

        for (size_t j = i + 1; j < cutter.points.size(); j++) {
            line_t edge2 = {
                .p1 = cutter.points[j],
                .p2 = cutter.points[(j + 1) % cutter.points.size()],
            };

            if (find_intersection(tmp, edge1, edge2, prev))
                return 0;
        }
    }

    return prev;
}

int scalar_prod(QPoint p1, QPoint p2)
{
    return p1.x() * p2.x() + p1.y() * p2.y();
}

bool cut(figure_t& res, figure_t fig, figure_t cutter)
{
    if (!cutter.exists) {
        return false;
    }

    int normal = is_convex(cutter);
    if (!normal)
        return false;

    for (size_t i = 0; i < cutter.points.size(); i++) {
        line_t cut_line = {
            .p1 = cutter.points[i],
            .p2 = cutter.points[(i + 1) % cutter.points.size()],
        };

        figure_t new_vis_fig;

        for (size_t j = 0; j < fig.points.size(); j++) {
            line_t vis_line = {
                .p1 = fig.points[j],
                .p2 = fig.points[(j + 1) % fig.points.size()],
            };

            QPoint inter;

            bool is_inter = find_intersection(inter, cut_line, vis_line, normal);
            if (is_inter) {
                new_vis_fig.points.push_back(inter);
            }

            bool is_vis = is_visible(cut_line, vis_line.p2, normal);
            if (is_vis)
                new_vis_fig.points.push_back(vis_line.p2);
        }

        fig = new_vis_fig;
    }

    res = fig;

    return true;
}

void draw_unfinished_fig(figure_t figure, QPainter& p)
{
    if (!figure.points.empty()) {
        for (size_t i = 0; i < figure.points.size() - 1; i++) {
            p.drawLine(figure.points[i], figure.points[(i + 1) % figure.points.size()]);
        }
    }
}

void draw_fig(figure_t figure, QPainter& p)
{
    if (!figure.exists) {
        draw_unfinished_fig(figure, p);
        return;
    }

    for (size_t i = 0; i < figure.points.size(); i++) {
        p.drawLine(figure.points[i], figure.points[(i + 1) % figure.points.size()]);
    }
}

void draw_figs(colors_t colors, bool progress_figures, std::vector<figure_t> figures, figure_t cutter, QPainter& p)
{
    for (size_t i = 0; i < figures.size(); i++) {
//        if (i == figures.size() - 1 && progress_figures)
//            continue;

        p.setPen(QPen(colors.invisible_color, 2));
        draw_fig(figures[i], p);

        figure_t sec;
        bool flag = cut(sec, figures[i], cutter);

        if (flag) {
            p.setPen(QPen(colors.visible_color, 2));
            draw_fig(sec, p);
        }
    }
}

void draw_all(colors_t colors, bool draw_figures, bool progress_figures, bool draw_figure, bool progress_figure, std::vector<figure_t> figures, figure_t figure, QGraphicsView* view)
{
    QImage image = QImage(view->geometry().width(),
                          view->geometry().height(),
                          QImage::Format_RGB32);

    QPainter p(&image);
    image.fill(QColor(255, 255, 255));

        p.setPen(QPen(colors.cutter_color, 2));
        draw_fig(figure, p);

    draw_figs(colors, progress_figures, figures, figure, p);

    QPixmap pixmap = QPixmap::fromImage(image);
    view->scene()->clear();
    view->scene()->addPixmap(pixmap);
}
