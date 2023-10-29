//
// Created by muhomorfus on 4/10/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QColorDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QWheelEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene();
    ui->canvas->setScene(scene);
    ui->canvas->scene()->clear();

    connect(ui->bgColorButton, SIGNAL(clicked()), SLOT(bgColorButtonClicked()));
    connect(ui->lineColorButton, SIGNAL(clicked()),
            SLOT(lineColorButtonClicked()));
    connect(ui->hiddenColorButton, SIGNAL(clicked()),
            SLOT(hiddenColorButtonClicked()));

    connect(ui->undoButton, SIGNAL(clicked()), SLOT(undoButtonClicked()));
    connect(ui->clearButton, SIGNAL(clicked()), SLOT(clearButtonClicked()));

    connect(ui->exitAction, SIGNAL(triggered()), SLOT(exitActionTriggered()));
    connect(ui->programInfoAction, SIGNAL(triggered()), SLOT(programInfoActionTriggered()));
    connect(ui->authorInfoAction, SIGNAL(triggered()), SLOT(authorInfoActionTriggered()));
}

MainWindow::~MainWindow() {
    delete ui->canvas->scene();

    delete ui;
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    windowShown();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
}

void MainWindow::windowShown() {
    setLabelColor(bgColor, ui->bgColorLabel);
    setLabelColor(lineColor, ui->lineColorLabel);
    setLabelColor(hiddenColor, ui->hiddenColorLabel);
}

void setLabelColor(QColor color, QLabel *label) {
    QImage image = QImage(label->geometry().width(), label->geometry().height(),
                          QImage::Format_RGB32);
    QPainter p(&image);

    p.setBrush(QColor(color));
    p.setPen(QPen(color, 1));
    QRect rect = QRect(0, 0, label->geometry().width(),
                       label->geometry().height());
    p.drawRect(rect);

    QPixmap pixmap = QPixmap::fromImage(image);
    label->clear();
    label->setPixmap(pixmap);
}

void MainWindow::bgColorButtonClicked() {
    QColorDialog dialog;
    dialog.setCurrentColor(bgColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    bgColor = color;
    setLabelColor(bgColor, ui->bgColorLabel);
//    drawAll();
}

void MainWindow::lineColorButtonClicked() {
    QColorDialog dialog;
    dialog.setCurrentColor(lineColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    lineColor = color;
    setLabelColor(lineColor, ui->lineColorLabel);
}

void MainWindow::hiddenColorButtonClicked() {
    QColorDialog dialog;
    dialog.setCurrentColor(hiddenColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    hiddenColor = color;
    setLabelColor(hiddenColor, ui->hiddenColorLabel);
}

void MainWindow::undoButtonClicked() {

}

void MainWindow::clearButtonClicked() {

}

void MainWindow::exitActionTriggered() {
    QApplication::quit();
}

void MainWindow::programInfoActionTriggered() {
    QMessageBox::about(this, "О программе", "Программа для всяких окружностей.\n");
}

void MainWindow::authorInfoActionTriggered() {
    QMessageBox box(this);
    box.setWindowTitle("Об авторе");
    box.setTextFormat(Qt::RichText);
    box.setText("Алеша Княжев, ИУ7-42Б.<br><br> <a href='https://github.com/muhomorfus'>GitHub</a>");
    box.exec();
}
