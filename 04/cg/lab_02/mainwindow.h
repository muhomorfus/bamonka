#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "point.h"
#include "figure.h"
#include "history.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum pointSelect {
    NONE,
    SELECT_ROTATE,
    SELECT_RESIZE,
};

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
    void on_cancelButton_clicked();

    void on_moveButton_clicked();

    void on_rotateButton_clicked();

    void on_resizeButton_clicked();

    void on_rotateCenterSelectButton_clicked();

    void on_resizeCenterSelectButton_clicked();

    void on_clearButton_clicked();

    void on_exitAction_triggered();

    void on_programInfoAction_triggered();

    void on_authorInfoAction_triggered();

private:
    void revert();
    QPoint toQPoint(point p);

    void drawFigure();
    void drawAxis(QPainter *painter);

    Ui::MainWindow *ui;

    point topLeft = point(-900, -900);
    point bottomRight = point(900, 900);

    figure elephant = figure();

    history events = history();

    pointSelect select = NONE;
};
#endif // MAINWINDOW_H
