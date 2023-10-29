#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "point.h"
#include "history.h"
#include "triangle.h"

#define POINT_SIZE 7
#define SELECT_RADIUS POINT_SIZE * 3 / 4
#define ARROW_SIZE 5
#define PADDING 10
#define MIN_VIEW_PX 100

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , topLeft(0, 0)
    , bottomRight(100, 100)
    , events()
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    ui->canvas->setScene(scene);

    selected = -1;
}

MainWindow::~MainWindow() {
    delete ui->canvas->scene();
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    drawPoints();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
   QMainWindow::resizeEvent(event);
   if (showSolution)
       drawSolution();
   else
       drawPoints();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    showSolution = false;

    int x = event->pos().x() - ui->canvas->geometry().x();
    int y = event->pos().y() - ui->canvas->geometry().y();

    bool inCanvas = x >= 0 && y >= 0 && x <= ui->canvas->geometry().width() && y <= ui->canvas->geometry().height();
    dropSelection();
    if (inCanvas) {
        for (int i = 0; i < points.size() && selected == -1; i++) {
            QPoint qtPoint = toQPoint(points[i]);
            if (abs(qtPoint.x() - x) <= SELECT_RADIUS && abs(qtPoint.y() - y) <= SELECT_RADIUS) {
                selectPoint(i);
            }
        }

        if (selected == -1) {
            QPoint qtPoint = QPoint(x, y);
            point p = point(qtPoint, topLeft, bottomRight, ui->canvas->geometry().width(), ui->canvas->geometry().height());
            addPoint(p);
            events.addEvent(EVENT_ADD_POINT);
        } else {
            drawPoints();
        }
    }
}

void MainWindow::addPoint(int x, int y) {
    addPoint(point(x, y));
}

void MainWindow::addPoint(point p) {
    if (p.x < topLeft.x)
        topLeft.x = p.x;
    else if (p.x > bottomRight.x)
        bottomRight.x = p.x;

    if (p.y < topLeft.y)
        topLeft.y = p.y;
    else if (p.y > bottomRight.y)
        bottomRight.y = p.y;

    points.push_back(p);

    int row = points.size() - 1;
    ui->pointsTable->insertRow(row);
    ui->pointsTable->setItem(row, 0, new QTableWidgetItem(QString::number(p.x)));
    ui->pointsTable->setItem(row, 1, new QTableWidgetItem(QString::number(p.y)));

    ui->statusbar->showMessage("Добавлена точка (" + QString::number(p.x) + ", " + QString::number(p.y) + ").");

    drawPoints();
}

QPoint MainWindow::toQPoint(point p) {
    return p.toQPoint(topLeft, bottomRight, ui->canvas->geometry().width(), ui->canvas->geometry().height());
}

void MainWindow::dropSelection() {
    ui->pointsTable->clearFocus();
    ui->pointsTable->clearSelection();

    selected = -1;
    drawPoints();
}

void MainWindow::selectPoint(int i) {
    ui->pointsTable->clearFocus();
    ui->pointsTable->clearSelection();
    ui->pointsTable->selectRow(i);

    selected = i;
    drawPoints();

    ui->statusbar->showMessage("Выделена точка (" + QString::number(points[i].x) + ", " + QString::number(points[i].y) + ").");
}

void MainWindow::on_addPointButton_clicked() {
    showSolution = false;
    dropSelection();

    bool validX, validY;
    int x = ui->inputX->text().toInt(&validX);
    int y = ui->inputY->text().toInt(&validY);

    ui->inputX->clear();
    ui->inputY->clear();

    if (!validX || !validY) {
        ui->statusbar->showMessage("Введите корректные координаты добавляемой точки.");
        return;
    }

    addPoint(x, y);
    events.addEvent(EVENT_ADD_POINT);
}

void MainWindow::drawAxis(QPainter *painter) {
    QColor color = QColorConstants::Black;
    painter->setPen(color);
    painter->setBrush(color);

    point center = point(0, 0);
    QPoint qtCenter = toQPoint(center);

    int yAxisMax = ui->canvas->geometry().height() - PADDING;
    painter->drawLine(qtCenter.x(), 0, qtCenter.x(), yAxisMax);
    painter->drawLine(qtCenter.x(), yAxisMax, qtCenter.x() - ARROW_SIZE, yAxisMax - ARROW_SIZE);
    painter->drawLine(qtCenter.x(), yAxisMax, qtCenter.x() + ARROW_SIZE, yAxisMax - ARROW_SIZE);

    int xAxisMax = ui->canvas->geometry().width() - PADDING;
    painter->drawLine(0, qtCenter.y(), xAxisMax, qtCenter.y());
    painter->drawLine(xAxisMax, qtCenter.y(), xAxisMax - ARROW_SIZE, qtCenter.y() - ARROW_SIZE);
    painter->drawLine(xAxisMax, qtCenter.y(), xAxisMax - ARROW_SIZE, qtCenter.y() + ARROW_SIZE);
}

void MainWindow::drawPoint(QPainter *painter, QPoint point, QColor color) {
    painter->setPen(color);
    painter->setBrush(color);

    int delta = POINT_SIZE / 2;
    QRect pointRect = QRect(point.x() - delta, point.y() - delta, POINT_SIZE, POINT_SIZE);
    painter->drawEllipse(pointRect);
}

void MainWindow::drawPoints() {
    QImage image = QImage(ui->canvas->geometry().width(), ui->canvas->geometry().height(), QImage::Format_RGB32);
    QPainter painter(&image);

    image.fill(QColorConstants::White);
    drawAxis(&painter);

    for (int i = 0; i < points.size(); i++)
    {
        QColor color = QColorConstants::Blue;
        if (i == selected)
            color = QColorConstants::Red;
        QPoint qtPoint = toQPoint(points[i]);
        drawPoint(&painter, qtPoint, color);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    ui->canvas->scene()->clear();
    ui->canvas->scene()->addPixmap(pixmap);
}

void MainWindow::drawTriangle(QPainter *painter, triangle t, QColor color) {
    painter->setPen(color);
    painter->setBrush(color);

    for (int i = 0; i < 3; i++)
        for (int j = i + 1; j < 3; j++)
        {
            QPoint first = toQPoint(t.points[i]);
            QPoint second = toQPoint(t.points[j]);
            painter->drawLine(first, second);
        }
}

void MainWindow::drawSolution() {
    dropSelection();

    triangle solution = findSolution(points);
    if (solution.empty) {
        ui->statusbar->showMessage("Нет решения.");
        return;
    }

    QImage image = QImage(ui->canvas->geometry().width(), ui->canvas->geometry().height(), QImage::Format_RGB32);
    QPainter painter(&image);

    image.fill(QColorConstants::White);
    drawAxis(&painter);

    for (int i = 0; i < points.size(); i++)
    {
        QPoint qtPoint = toQPoint(points[i]);
        drawPoint(&painter, qtPoint, QColorConstants::Blue);
    }

    drawTriangle(&painter, solution, QColorConstants::Blue);

    ui->statusbar->showMessage("Треугольник построен.");
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->canvas->scene()->clear();
    ui->canvas->scene()->addPixmap(pixmap);
}

void MainWindow::on_pointsTable_cellClicked(int row, int column) {
    showSolution = false;
    selectPoint(row);
}


void MainWindow::on_deletePointButton_clicked() {
    showSolution = false;
    if (selected == -1)
    {
        ui->statusbar->showMessage("Выберите точку для удаления.");
        return;
    }

    int i = selected;
    point deleted = points[i];
    points.erase(points.begin() + i);
    deletedPoints.push_back(deleted);

    ui->pointsTable->removeRow(i);

    events.addEvent(EVENT_DELETE_POINT);

    ui->statusbar->showMessage("Точка (" + QString::number(deleted.x) + ", " + QString::number(deleted.y) + ") удалена.");
    dropSelection();
    drawPoints();
}


void MainWindow::on_deleteAllButton_clicked() {
    showSolution = false;
    while (!points.empty()) {
        point deleted = points.back();
        points.pop_back();
        deletedPoints.push_back(deleted);
        ui->pointsTable->removeRow(0);

        events.addEvent(EVENT_DELETE_POINT);
    }

    ui->statusbar->showMessage("Все точки удалены.");
    dropSelection();
    drawPoints();
}


void MainWindow::on_showSolutionButton_clicked() {
    showSolution = true;
    drawSolution();
}


void MainWindow::scale() {
    showSolution = false;

    if (points.empty()) {
        topLeft = point(0, 0);
        bottomRight = point(100, 100);
    } else {
        topLeft = points[0];
        bottomRight = points[0];

        for (int i = 0; i < points.size(); i++) {
            if (points[i].x > bottomRight.x)
                bottomRight.x = points[i].x;
            else if (points[i].x < topLeft.x)
                topLeft.x = points[i].x;

            if (points[i].y > bottomRight.y)
                bottomRight.y = points[i].y;
            else if (points[i].y < topLeft.y)
                topLeft.y = points[i].y;
        }

        if (bottomRight.x - topLeft.x < MIN_VIEW_PX) {
            int addition = (MIN_VIEW_PX - (bottomRight.x - topLeft.x)) / 2;
            bottomRight.x += addition;
            topLeft.x -= addition;
        }

        if (bottomRight.y - topLeft.y < MIN_VIEW_PX) {
            int addition = (MIN_VIEW_PX - (bottomRight.y - topLeft.y)) / 2;
            bottomRight.y += addition;
            topLeft.y -= addition;
        }
    }

    drawPoints();
}

void MainWindow::on_cutCanvasButton_clicked() {
    scale();
    ui->statusbar->showMessage("Холст обрезан.");
}


void MainWindow::on_cancelButton_clicked() {
    if (events.empty()) {
        ui->statusbar->showMessage("Нечего отменять.");
        return;
    }

    enum event last = events.getLastEvent();
    if (last == EVENT_ADD_POINT) {
        points.pop_back();
        ui->pointsTable->removeRow(points.size());
    } else if (last == EVENT_DELETE_POINT) {
        points.push_back(deletedPoints.back());
        deletedPoints.pop_back();

        int i = points.size() - 1;
        ui->pointsTable->insertRow(i);
        ui->pointsTable->setItem(i, 0, new QTableWidgetItem(QString::number(points[i].x)));
        ui->pointsTable->setItem(i, 1, new QTableWidgetItem(QString::number(points[i].y)));

        scale();
    }
    events.cancelLastEvent();

    drawPoints();
    ui->statusbar->showMessage("Действия отменено.");
}


void MainWindow::on_exitAction_triggered() {
    QApplication::quit();
}



void MainWindow::on_programInfoAction_triggered()
{
    QMessageBox::about(this, "О программе", "Программа для поиска решения геометрической задачи. Вариант 11.\n\n"
                             "Из заданного множества точек на плоскости выбрать три разные точки A, B,"
                                                  " C так, чтобы внутри треугольника АВС содержалось максимальное"
                                                  " количество точек этого множества, а по каждой стороне вне"
                                                  " треугольника было одинаковое количество точек.");
}


void MainWindow::on_authorInfoAction_triggered() {
    QMessageBox box(this);
    box.setWindowTitle("Об авторе");
    box.setTextFormat(Qt::RichText);
    box.setText("Алеша Княжев, ИУ7-42Б.<br><br> <a href='https://github.com/muhomorfus'>GitHub</a>");
    box.exec();
}

