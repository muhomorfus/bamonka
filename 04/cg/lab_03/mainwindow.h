#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QColor>
#include <QLabel>

#include "task.h"
#include "history.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

void setLabelColor(QColor color, QLabel* label);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showEvent(QShowEvent* e);
    void resizeEvent(QResizeEvent* e);
    void wheelEvent(QWheelEvent* e);

private slots:
    void on_lineColorButton_clicked();

    void on_bgColorButton_clicked();

    void draw();

    void on_addLineButton_clicked();

    void on_addSpectreButton_clicked();

    void on_cancelButton_clicked();

    void on_clearButton_clicked();

    void on_showTimeButton_clicked();

    void on_showStepsButton_clicked();

    void on_exitAction_triggered();

    void on_programInfoAction_triggered();

    void on_authorInfoAction_triggered();

private:
    Ui::MainWindow *ui;

    QColor bgColor = QColor(255, 255, 255);
    QColor lineColor = QColor(0, 0, 0);

    History history = History();

    std::vector<Object> objects;

    int numScaled = 0;
};
#endif // MAINWINDOW_H
