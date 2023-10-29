#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <stack>

#include "point.h"
#include "history.h"
#include "triangle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* event);

private slots:
    void on_addPointButton_clicked();

    void drawPoint(QPainter *painter, QPoint point, QColor color);
    void drawPoints();
    void drawAxis(QPainter *painter);
    void drawTriangle(QPainter *painter, triangle t, QColor color);
    void drawSolution();

    void scale();

    void addPoint(int x, int y);
    void addPoint(point p);

    void selectPoint(int i);
    void dropSelection();

    void on_pointsTable_cellClicked(int row, int column);

    void on_deletePointButton_clicked();

    void on_deleteAllButton_clicked();

    void on_showSolutionButton_clicked();

    void on_cutCanvasButton_clicked();

    void on_cancelButton_clicked();

    void on_exitAction_triggered();

    void on_programInfoAction_triggered();

    void on_authorInfoAction_triggered();

    QPoint toQPoint(point p);

private:
    Ui::MainWindow *ui;

    point topLeft;
    point bottomRight;

    std::vector<point> points;
    std::vector<point> deletedPoints;

    history events;

    int selected;

    bool showSolution = false;
};
#endif // MAINWINDOW_H
