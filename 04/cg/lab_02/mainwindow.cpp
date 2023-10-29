#include <QMessageBox>
#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "point.h"
#include "figure.h"

#include "history.h"


#define ARROW_SIZE 5
#define PADDING 10

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    ui->canvas->setScene(scene);
    ui->canvas->setRenderHints(QPainter::Antialiasing);
}

MainWindow::~MainWindow() {
    delete ui->canvas->scene();
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
   drawFigure();
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    drawFigure();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    int x = event->pos().x() - ui->canvas->geometry().x();
    int y = event->pos().y() - ui->canvas->geometry().y();

    bool inCanvas = x >= 0 && y >= 0 && x <= ui->canvas->geometry().width() && y <= ui->canvas->geometry().height();

    if (!inCanvas) {
        select = NONE;
        return;
    }

    QPoint qtPoint(x, y);
    point p = point(qtPoint, topLeft, bottomRight, ui->canvas->geometry().width(), ui->canvas->geometry().height());
    switch (select) {
    case NONE:
        return;
    case SELECT_ROTATE:
        ui->rotateCenterX->setText(QString::number(p.x));
        ui->rotateCenterY->setText(QString::number(p.y));
        break;
    case SELECT_RESIZE:
        ui->resizeCenterX->setText(QString::number(p.x));
        ui->resizeCenterY->setText(QString::number(p.y));
        break;
    default:
        break;
    }

    select = NONE;
}

QPoint MainWindow::toQPoint(point p) {
    return p.toQPoint(topLeft, bottomRight, ui->canvas->geometry().width(), ui->canvas->geometry().height());
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

void MainWindow::drawFigure() {
    QImage image = QImage(ui->canvas->geometry().width(), ui->canvas->geometry().height(), QImage::Format_RGB32);
    QPainter painter(&image);

    image.fill(QColorConstants::White);
    drawAxis(&painter);

    elephant.draw(&painter, QColor(68, 148, 74), topLeft, bottomRight, ui->canvas->geometry().width(), ui->canvas->geometry().height());

    QPixmap pixmap = QPixmap::fromImage(image);
    ui->canvas->scene()->clear();
    ui->canvas->scene()->addPixmap(pixmap);
}

void MainWindow::revert() {
    eventInfo last = events.getLastEvent();

    switch (last.type) {
    case EMPTY:
        return;
    case EVENT_MOVE:
        elephant.revertMove(last.info.move.xShift, last.info.move.yShift);
        break;
    case EVENT_ROTATE:
        elephant.revertRotate(last.info.rotate.base, last.info.rotate.angle);
        break;
    case EVENT_RESIZE:
        elephant.revertResize(last.info.resize.base, last.info.resize.xScale, last.info.resize.yScale);
        break;
    }

    events.cancelLastEvent();
}

void MainWindow::on_cancelButton_clicked() {
    revert();

    drawFigure();
}


void MainWindow::on_moveButton_clicked() {
    bool validXShift, validYShift;
    int x = ui->moveXShift->text().toInt(&validXShift);
    int y = ui->moveYShift->text().toInt(&validYShift);

    if (!validXShift || !validYShift) {
        QMessageBox::warning(this, "Ошибка ввода", "Сдвиг введен неверно");
        ui->moveXShift->clear();
        ui->moveYShift->clear();
        return;
    }

    elephant.move(x, y);
    drawFigure();

    events.addMove(x, y);

    ui->moveXShift->clear();
    ui->moveYShift->clear();
}


void MainWindow::on_rotateButton_clicked() {
    bool validXCenter, validYCenter;
    int x = ui->rotateCenterX->text().toInt(&validXCenter);
    int y = ui->rotateCenterY->text().toInt(&validYCenter);

    if (!validXCenter || !validYCenter) {
        QMessageBox::warning(this, "Ошибка ввода", "Координаты введены неверно");
        ui->rotateCenterX->clear();
        ui->rotateCenterX->clear();
        return;
    }

    bool validAngle;
    int angle = ui->rotateAngle->text().toInt(&validAngle);
    if (!validAngle) {
        QMessageBox::warning(this, "Ошибка ввода", "Угол введен неверно");
        ui->rotateAngle->clear();
        return;
    }

    elephant.rotate(point(x, y), angle);
    drawFigure();

    events.addRotate(point(x, y), angle);

    ui->rotateCenterX->clear();
    ui->rotateCenterY->clear();
    ui->rotateAngle->clear();
}


void MainWindow::on_resizeButton_clicked() {
    bool validXCenter, validYCenter;
    int x = ui->resizeCenterX->text().toInt(&validXCenter);
    int y = ui->resizeCenterY->text().toInt(&validYCenter);

    if (!validXCenter || !validYCenter) {
        QMessageBox::warning(this, "Ошибка ввода", "Координаты введены неверно");
        ui->resizeCenterX->clear();
        ui->resizeCenterX->clear();
        return;
    }

    bool validXRatio, validYRatio;
    double xRatio = ui->resizeXRatio->text().toDouble(&validXRatio);
    double yRatio = ui->resizeYRatio->text().toDouble(&validYRatio);
    if (!validXRatio || !validYRatio) {
        QMessageBox::warning(this, "Ошибка ввода", "Коэффициенты введены неверно");
        ui->resizeXRatio->clear();
        ui->resizeYRatio->clear();
        return;
    }

    elephant.resize(point(x, y), xRatio, yRatio);
    drawFigure();

    events.addResize(point(x, y), xRatio, yRatio);

    ui->resizeCenterX->clear();
    ui->resizeCenterY->clear();
    ui->resizeXRatio->clear();
    ui->resizeYRatio->clear();
}


void MainWindow::on_rotateCenterSelectButton_clicked() {
    select = SELECT_ROTATE;
}


void MainWindow::on_resizeCenterSelectButton_clicked() {
    select = SELECT_RESIZE;
}


void MainWindow::on_clearButton_clicked() {
    while (!events.empty())
        revert();
    drawFigure();
}

void MainWindow::on_exitAction_triggered() {
    QApplication::quit();
}

void MainWindow::on_programInfoAction_triggered()
{
    QMessageBox::about(this, "О программе", "Программа для всяких поворотов-разворотов. Вариант 11. Зеленый слоник.\n");
}

void MainWindow::on_authorInfoAction_triggered() {
    QMessageBox box(this);
    box.setWindowTitle("Об авторе");
    box.setTextFormat(Qt::RichText);
    box.setText("Алеша Княжев, ИУ7-42Б.<br><br> <a href='https://github.com/muhomorfus'>GitHub</a>");
    box.exec();
}

