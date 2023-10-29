#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

#include "task.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void showEvent(QShowEvent* ev);

    void close();

    void resizeEvent(QResizeEvent* event);

    void mousePressEvent(QMouseEvent* event);

    void setLabelColor(QColor color, QLabel* label);

private slots:

    void authorInfo_mbox();

    void progInfo_mbox();

    void on_fcButton_clicked();

    void on_lcButton_clicked();

    void on_ccButton_clicked();

    void on_addFigureButton_clicked();

    void on_backButton_clicked();

    void on_clearButton_clicked();

    void on_deleteFigureButton_clicked();

    void on_closeFigureButton_clicked();

    void on_closeLineButton_clicked();

private:
    Ui::MainWindow* ui;

    std::vector<event_t> lastevents;
    std::vector<figure_t> figures;
    figure_t cutter = {
        .exists = false,
    };

    bool draw_figure = true;
    bool draw_cutter = false;

    bool progress_figure = false;
    bool progress_cutter = false;

    std::vector<figure_t> deleted_cutters;

    colors_t colors = {
        .cutter_color = QColor(0, 0, 0),
        .invisible_color = QColor(180, 180, 180),
        .visible_color = QColor(255, 100, 100),
    };
};
#endif // MAINWINDOW_H
