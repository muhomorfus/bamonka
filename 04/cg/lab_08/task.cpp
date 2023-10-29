
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

void get_d_k_b(int ax, int ay, int cx, int cy, double& d, double& k, double& b)
{
    // Коэффициенты прямой АС
    // Если точки A и С лежат на одной вертикальной прямой
    if (cx == ax) {
        k = 1;
        b = -cx;
        d = 0;
    } else {
        k = 1.0 * (cy - ay) / (cx - ax);
        b = cy - (k * cx);
        d = 1;
    }
}

bool are_crossed(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy)
{
    double d_ab, k_ab, b_ab;
    double d_cd, k_cd, b_cd;
    get_d_k_b(ax, ay, bx, by, d_ab, k_ab, b_ab);
    get_d_k_b(cx, cy, dx, dy, d_cd, k_cd, b_cd);

    if (abs(k_ab - k_cd) < EPS)
        return false;
    double x = (b_cd - b_ab) / (k_ab - k_cd);
    double y;
    if (d_cd < EPS)
        y = (k_ab * x + b_ab);
    else if (d_ab < EPS)
        y = (k_cd * x + b_cd);
    else
        y = (k_ab * x + b_ab);
    int b1 = ax;
    int b2 = bx;
    ax = std::max(b1, b2);
    bx = std::min(b1, b2);
    b1 = ay;
    b2 = by;
    ay = std::max(b1, b2);
    by = std::min(b1, b2);

    if ((abs(bx - x) < EPS) || (abs(ax - x) < EPS) || (abs(by - y) < EPS) || (abs(ay - y) < EPS))
        return false;
    if (bx < x && x < ax && by < y && y < ay)
        return true;
    else
        return false;
}

bool check_cross(figure_t cutter)
{
    size_t len = cutter.points.size();
    bool f;
    for (size_t i = 0; i < len - 1; i++)
        for (size_t j = i + 1; j < len; j++) {
            if (j == len - 1) {
                f = are_crossed(cutter.points[i].x(), cutter.points[i].y(), cutter.points[i + 1].x(), cutter.points[i + 1].y(),
                                cutter.points[j].x(), cutter.points[j].y(), cutter.points[0].x(), cutter.points[0].y());
                if (f)
                    return true;
            } else {
                f = are_crossed(cutter.points[i].x(), cutter.points[i].y(), cutter.points[i + 1].x(), cutter.points[i + 1].y(),
                                cutter.points[j].x(), cutter.points[j].y(), cutter.points[j + 1].x(), cutter.points[j + 1].y());
                if (f)
                    return true;
            }
        }
    return false;
}

int is_convex(figure_t cutter)
{
    if (cutter.points.size() < 3)
        return 1;

    size_t len = cutter.points.size();

    QPoint v1 = get_vector(cutter.points[len - 1], cutter.points[0]);
    QPoint v2 = get_vector(cutter.points[len - 2], cutter.points[len - 1]);

    int prev = sign(vector_prod(v1, v2));

    for (size_t i = 1; i < len - 2; i++) {
        v1 = get_vector(cutter.points[i - 1], cutter.points[i]);
        if (i == 1)
            v2 = get_vector(cutter.points[len - 1], cutter.points[i - 1]);
        else
            v2 = get_vector(cutter.points[i - 2], cutter.points[i - 1]);

        int cur = sign(vector_prod(v1, v2));
        if (prev != cur)
            return 0;
        prev = cur;
    }

    if (check_cross(cutter))
        return 0;

    return prev;
}

void parallel_point(int& x, int& y, figure_t cutter, std::vector<line_t> lines)
{
    double angle = atan(((double)lines[lines.size() - 1].p1.y() - y) / ((double)lines[lines.size() - 1].p1.x() - x));

    double min_dif = 0, rib_angle = 0;
    int min_ind = 0;
    for (size_t i = 0; i < cutter.points.size(); i++) {
        size_t p1_i = i;
        size_t p2_i = (i + 1) % cutter.points.size();

        rib_angle = atan(((double)cutter.points[p1_i].y() - cutter.points[p2_i].y()) / ((double)cutter.points[p1_i].x() - cutter.points[p2_i].x()));

        if (i == 0 || fabs(fabs(angle - rib_angle) - M_PI_2) > min_dif) {
            min_dif = fabs(fabs(angle - rib_angle) - M_PI_2);
            min_ind = i;
        }
    }

    size_t p1_i = min_ind;
    size_t p2_i = (min_ind + 1) % cutter.points.size();

    rib_angle = atan(((double)cutter.points[p1_i].y() - cutter.points[p2_i].y()) / ((double)cutter.points[p1_i].x() - cutter.points[p2_i].x()));
    min_dif = angle - rib_angle;

    double dist = sqrt(pow((lines[lines.size() - 1].p1.x() - x), 2) + pow((lines[lines.size() - 1].p1.y() - y), 2));
    dist *= cos(min_dif);

    if (x < lines[lines.size() - 1].p1.x())
        dist *= -1;

    x = lines[lines.size() - 1].p1.x() + dist * cos(rib_angle);
    y = lines[lines.size() - 1].p1.y() + dist * sin(rib_angle);

    QPoint n_in = QPoint(cutter.points[p2_i].y() - cutter.points[p1_i].y(), -(cutter.points[p2_i].x() - cutter.points[p1_i].x()));

    if (rib_angle)
        x -= (x - lines[lines.size() - 1].p1.x() + (y - lines[lines.size() - 1].p1.y()) * n_in.y() / n_in.x());

    return;
}

int scalar_prod(QPoint p1, QPoint p2)
{
    return p1.x() * p2.x() + p1.y() * p2.y();
}

bool cut_line(line_t line, figure_t cut, int conv, QPainter& p)
{
    int m = cut.points.size();
    QPoint D = line.p2 - line.p1;
    double t_low = 0, t_high = 1;
    for (int i = 0; i < m; i++) {
        size_t ind = (i + 1) % m;
        QPoint f = cut.points[i], p2 = cut.points[ind];
        QPoint n_in = QPoint(conv * (p2.y() - f.y()), -conv * (p2.x() - f.x()));
        QPoint W = line.p1 - f;
        double Wsc = scalar_prod(W, n_in);
        double Dsc = scalar_prod(D, n_in);

        if (Dsc) {
            double t = -Wsc / Dsc;
            if (Dsc > 0) {
                if (t > 1)
                    return false;
                else if (t > t_low) {
                    t_low = t;
                }
            } else {
                if (t < 0)
                    return false;
                else if (t < t_high) {
                    t_high = t;
                }
            }
        } else if (Wsc < 0)
            return false;
    }
    if (t_low > t_high)
        return false;

    QPoint p1 = line.p1 + (line.p2 - line.p1) * t_low;
    QPoint p2 = line.p1 + (line.p2 - line.p1) * t_high;

    p.drawLine(p1, p2);
    return true;
}

void kirus_back(line_t line, figure_t cutter, colors_t colors, QPainter& p)
{
    p.setPen(QPen(colors.invisible_color, 2));
    p.drawLine(line.p1, line.p2);

    int conv = is_convex(cutter);

    if (cutter.exists) {
        p.setPen(QPen(colors.visible_color, 2));
        cut_line(line, cutter, conv, p);
    }
}

void draw_lines(colors_t colors, std::vector<line_t> lines, figure_t figure, QPainter& p)
{
    p.setPen(QPen(colors.invisible_color, 2));

    for (size_t i = 0; i < lines.size(); i++) {
        //        if (i == lines.size() - 1 && progress_line)
        //            continue;

        p.drawLine(lines[i].p1, lines[i].p2);
        kirus_back(lines[i], figure, colors, p);
    }
}

void draw_fig(colors_t colors, figure_t figure, QPainter& p)
{
    p.setPen(QPen(colors.cutter_color, 2));

    if (figure.exists) {
        for (size_t i = 0; i < figure.points.size(); i++) {
            p.drawLine(figure.points[i], figure.points[(i + 1) % figure.points.size()]);
        }
    }
    else if (figure.points.size() > 1) {
        for (size_t i = 0; i < figure.points.size() - 1; i++) {
            p.drawLine(figure.points[i], figure.points[i + 1]);
        }
    }
}

void draw_all(colors_t colors, bool draw_line, bool progress_line, bool draw_figure, bool progress_figure, std::vector<line_t> lines, figure_t figure, QGraphicsView* view)
{
    QImage image = QImage(view->geometry().width(),
                          view->geometry().height(),
                          QImage::Format_RGB32);

    QPainter p(&image);
    image.fill(QColor(255, 255, 255));

    draw_lines(colors, lines, figure, p);
    draw_fig(colors, figure, p);

    QPixmap pixmap = QPixmap::fromImage(image);
    view->scene()->clear();
    view->scene()->addPixmap(pixmap);
}
