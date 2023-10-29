//
// Created by muhomorfus on 4/10/22.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QWidget>
#include <QMainWindow>
#include <QLabel>

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

public slots:

    void windowShown();

    void bgColorButtonClicked();

    void lineColorButtonClicked();

    void hiddenColorButtonClicked();

    void undoButtonClicked();

    void clearButtonClicked();

    void exitActionTriggered();

    void programInfoActionTriggered();

    void authorInfoActionTriggered();

private:
    Ui::MainWindow *ui;

    QColor bgColor = QColor(255, 0, 0);
    QColor lineColor = QColor(0, 0, 0);
    QColor hiddenColor = QColor(100, 100, 100);

    int scaled = 0;

//    void drawAll();
};

#endif //MAINWINDOW_H
