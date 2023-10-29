#include <QColorDialog>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->aboutAuthorAction, SIGNAL(triggered()), this, SLOT(authorInfo_mbox()));
    connect(ui->aboutProgAction, SIGNAL(triggered()), this, SLOT(progInfo_mbox()));
    connect(ui->exitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->fcButton, SIGNAL(clicked()), this, SLOT(on_fcButton_clicked()));
    connect(ui->lcButton, SIGNAL(clicked()), this, SLOT(on_lcButton_clicked()));
    connect(ui->ccButton, SIGNAL(clicked()), this, SLOT(on_ccButton_clicked()));

    connect(ui->addFigureButton, SIGNAL(clicked()), this, SLOT(on_addFigureButton_clicked()));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(on_clearButton_clicked()));
    connect(ui->deleteFigureButton, SIGNAL(clicked()), this, SLOT(on_deleteFigureButton_clicked()));
    connect(ui->closeFigureButton, SIGNAL(clicked()), this, SLOT(on_closeFigureButton_clicked()));

    auto* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->clear();

    centralWidget()->setMouseTracking(true);
    ui->graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);
}

MainWindow::~MainWindow() // деструктор
{
    delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::showEvent(QShowEvent* ev) // когда окно полностью сконструировано
{
    QMainWindow::showEvent(ev);

    setLabelColor(colors.cutter_color, ui->fcLabel);
    setLabelColor(colors.invisible_color, ui->lcLabel);
    setLabelColor(colors.visible_color, ui->ccLabel);

    ui->deleteFigureButton->setDisabled(true);
    ui->closeFigureButton->setDisabled(true);
}

void MainWindow::authorInfo_mbox()
{
    QMessageBox::information(this, "Об авторе", "Княжев Алеша, ИУ7-42Б.");
}

void MainWindow::progInfo_mbox()
{
    QMessageBox::information(this, "О программе", "Отсечение отрезков произвольным выпуклым отсекателем с помощью алгоритма Кири-Узбека.");
}

void MainWindow::close()
{
    QApplication::quit();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::setLabelColor(QColor color, QLabel* label)
{
    QImage image = QImage(label->geometry().width(), label->geometry().height(),
        QImage::Format_RGB32);
    QPainter p(&image);

    p.setBrush(QColor(color));
    p.setPen(QPen(color, 1));
    QRect l_col_rect = QRect(0, 0, label->geometry().width(), label->geometry().height());
    p.drawRect(l_col_rect);

    QPixmap pixmap = QPixmap::fromImage(image);
    label->clear();
    label->setPixmap(pixmap);
}

void MainWindow::on_fcButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(colors.cutter_color);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;
    colors.cutter_color = color;
    setLabelColor(color, ui->fcLabel);

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::on_lcButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(colors.invisible_color);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;
    colors.invisible_color = color;
    setLabelColor(color, ui->lcLabel);

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::on_ccButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(colors.visible_color);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;
    colors.visible_color = color;
    setLabelColor(color, ui->ccLabel);

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    int canv_x = event->pos().x() - ui->graphicsView->x();
    int canv_y = event->pos().y() - ui->graphicsView->y() - ui->menubar->geometry().height();

    if (draw_line && progress_line) {
        Qt::KeyboardModifiers key = QApplication::queryKeyboardModifiers();
        if (figure.exists && key == Qt::ControlModifier)
            parallel_point(canv_x, canv_y, figure, lines);

        lines.back().p2 = QPoint(canv_x, canv_y);
    }

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    int canv_x = event->pos().x() - ui->graphicsView->x();
    int canv_y = event->pos().y() - ui->graphicsView->y() - ui->menubar->geometry().height();

    if (canv_x < 0 || canv_y < 0 || canv_x > ui->graphicsView->width() || canv_y > ui->graphicsView->height()) {
        return;
    }

    if (figure.exists)
        draw_figure = false;

    if (draw_figure && progress_figure) {
        figure.points.push_back(QPoint(canv_x, canv_y));
        if (!is_convex(figure)) {
            QMessageBox::warning(this, "Ошибка", "Многоугольник не выпуклый.");
            figure.points.pop_back();
            return;
        }

        draw_line = false;

        progress_line = false;
        if (figure.points.size() >= 3)
            ui->closeFigureButton->setEnabled(true);
        ui->deleteFigureButton->setEnabled(true);
    } else if (draw_figure) {
        figure.points.push_back(QPoint(canv_x, canv_y));

        progress_figure = true;
    } else if (draw_line && progress_line) {
        Qt::KeyboardModifiers key = QApplication::queryKeyboardModifiers();
        if (figure.exists && key == Qt::ControlModifier)
            parallel_point(canv_x, canv_y, figure, lines);

        lines.back().p2 = QPoint(canv_x, canv_y);

        progress_line = false;
        lastevents.push_back(EVENT_LINE);
    } else if (draw_line) {
        line_t line = {
            .p1 = QPoint(canv_x, canv_y),
            .p2 = QPoint(canv_x, canv_y),
        };
//        start_points.push_back(line.p1);
        lines.push_back(line);

        progress_line = true;
    }

    if (figure.exists) {
        ui->addFigureButton->setDisabled(true);
        ui->closeFigureButton->setDisabled(true);
    } else {
        ui->addFigureButton->setEnabled(true);
        if (draw_figure && progress_figure && figure.points.size() < 3)
            ui->closeFigureButton->setDisabled(true);
    }

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::on_addFigureButton_clicked()
{
    if (figure.exists)
        return;
    draw_figure = true;
    draw_line = false;
    ui->closeFigureButton->setDisabled(true);
    ui->deleteFigureButton->setDisabled(true);
}

void MainWindow::on_backButton_clicked()
{
    if (draw_line && progress_line) {
        lines.pop_back();
        progress_line = false;
    } else if (draw_figure) {
        draw_figure = false;
        figure.points.clear();
        progress_figure = false;
    } else if (lastevents.empty()) {
        return;
    } else if (lastevents.back() == EVENT_LINE) {
        lines.pop_back();
        draw_line = true;

        lastevents.pop_back();
    } else if (lastevents.back() == EVENT_FIGURE) {
        figure.exists = false;
        ui->addFigureButton->setEnabled(true);
        ui->closeFigureButton->setDisabled(true);

        figure.points.clear();
        lastevents.pop_back();
    } else if (lastevents.back() == EVENT_DELETE_FIGURE) {
        figure = deleted_figures.back();
        deleted_figures.pop_back();

        lastevents.pop_back();

        if (deleted_figures.empty()) {
            ui->deleteFigureButton->setDisabled(true);
            ui->closeFigureButton->setDisabled(true);
        }
    }

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::on_clearButton_clicked()
{
    lastevents.clear();
    figure.exists = false;
    figure.points.clear();
    deleted_figures.clear();
    lines.clear();

    draw_figure = false;
    draw_line = true;

    progress_figure = false;
    progress_line = false;

    ui->addFigureButton->setEnabled(true);
    ui->deleteFigureButton->setDisabled(true);
    ui->closeFigureButton->setDisabled(true);

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::on_deleteFigureButton_clicked()
{
    deleted_figures.push_back(figure);
    figure.exists = false;
    figure.points.clear();
    ui->addFigureButton->setEnabled(true);
    ui->closeFigureButton->setDisabled(true);

    lastevents.push_back(EVENT_DELETE_FIGURE);

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}

void MainWindow::on_closeFigureButton_clicked()
{
    if (figure.points.size() < 3)
        return;

    figure.exists = true;

    draw_figure = false;
    progress_figure = false;
    draw_line = true;

    ui->addFigureButton->setDisabled(true);
    ui->closeFigureButton->setDisabled(true);

    lastevents.push_back(EVENT_FIGURE);

    draw_all(colors, draw_line, progress_line, draw_figure, progress_figure, lines, figure, ui->graphicsView);
}
