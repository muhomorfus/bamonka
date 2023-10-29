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

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void showEvent(QShowEvent* ev) override;

    void close();

    void resizeEvent(QResizeEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

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

private:
    Ui::MainWindow* ui;

    std::vector<event_t> lastevents;
    std::vector<line_t> lines;
    figure_t figure = {
        .exists = false,
    };

    bool draw_figure = false;
    bool draw_line = true;

    bool progress_figure = false;
    bool progress_line = false;

    std::vector<figure_t> deleted_figures;

    colors_t colors = {
        .cutter_color = QColor(0, 0, 0),
        .invisible_color = QColor(180, 180, 180),
        .visible_color = QColor(255, 100, 100),
    };
};
#endif // MAINWINDOW_H
