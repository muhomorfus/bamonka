//
// Created by muhomorfus on 4/10/22.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QWidget>
#include <QMainWindow>
#include <QLabel>

#include "task.h"
#include "errors.h"

void setLabelColor(QColor color, QLabel *label);

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void showEvent(QShowEvent *event);

    void resizeEvent(QResizeEvent *event);

    bool eventFilter(QObject *object, QEvent *event);

    void wheelEvent(QWheelEvent *event);

public slots:

    void windowShown();

    void typeBoxIndexChanged(int index);

    void bgColorButtonClicked();

    void lineColorButtonClicked();

    void drawButtonClicked();

    void undoButtonClicked();

    void clearButtonClicked();

    void timeButtonClicked();

    void exitActionTriggered();

    void programInfoActionTriggered();

    void authorInfoActionTriggered();

private:
    Ui::MainWindow *ui;

    QColor bgColor = QColor(255, 255, 255);
    QColor lineColor = QColor(0, 0, 0);

    std::vector<Object> objects;

    int scaled = 0;

    void drawAll();

    Error readObject(Object &object);
};

#endif //MAINWINDOW_H
