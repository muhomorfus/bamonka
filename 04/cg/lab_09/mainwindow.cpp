#include <QColorDialog>
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
    connect(ui->closeLineButton, SIGNAL(clicked()), this, SLOT(on_closeLineButton_clicked()));

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->clear();
}

MainWindow::~MainWindow() // деструктор
{
    delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::showEvent(QShowEvent* ev)
{
    QMainWindow::showEvent(ev);

    setLabelColor(colors.cutter_color, ui->fcLabel);
    setLabelColor(colors.invisible_color, ui->lcLabel);
    setLabelColor(colors.visible_color, ui->ccLabel);

    ui->deleteFigureButton->setDisabled(true);
    ui->closeFigureButton->setDisabled(true);
    ui->closeLineButton->setDisabled(true);
}

void MainWindow::authorInfo_mbox()
{
    QMessageBox::information(this, "Об авторе", "Княжев Алеша, ИУ7-42Б.");
}

void MainWindow::progInfo_mbox()
{
    QMessageBox::information(this, "О программе", "Отсечение отрезков произвольным выпуклым отсекателем с помощью алгоритма Сазерленда-Ходжмена.");
}

void MainWindow::close()
{
    QApplication::quit();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
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

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
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

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
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

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    int canv_x = event->pos().x() - ui->graphicsView->x();
    int canv_y = event->pos().y() - ui->graphicsView->y() - ui->menubar->geometry().height();

    if (canv_x < 0 || canv_y < 0 || canv_x > ui->graphicsView->width() || canv_y > ui->graphicsView->height()) {
        return;
    }

    if (cutter.exists)
        draw_cutter = false;

    if (draw_cutter && progress_cutter) {
        if (std::find(cutter.points.begin(), cutter.points.end(), QPoint(canv_x, canv_y)) != cutter.points.end()) {
            QMessageBox::warning(this, "Ошибка", "Хватит тыкать в одну точку как дебил!");
            return;
        }

        cutter.points.push_back(QPoint(canv_x, canv_y));

        if (!is_convex(cutter)) {
            QMessageBox::warning(this, "Ошибка", "Многоугольник не выпуклый.");
            cutter.points.pop_back();
            return;
        }

        draw_figure = false;

        progress_figure = false;
        if (cutter.points.size() >= 3)
            ui->closeFigureButton->setEnabled(true);
        ui->deleteFigureButton->setEnabled(true);
    } else if (draw_cutter) { // start cutter
        cutter.points.push_back(QPoint(canv_x, canv_y));

        progress_cutter = true;
    } else if (draw_figure && progress_figure) { // continue figure
        figures.back().points.push_back(QPoint(canv_x, canv_y));
        if (figures.back().points.size() > 2)
            ui->closeLineButton->setEnabled(true);
    } else if (draw_figure) { // start figure
        figure_t figure = {};
        figure.points.push_back(QPoint(canv_x, canv_y));
        figures.push_back(figure);

        progress_figure = true;
    }

    if (cutter.exists) {
        ui->addFigureButton->setDisabled(true);
        ui->closeFigureButton->setDisabled(true);
    } else {
        ui->addFigureButton->setEnabled(true);
        if (draw_figure && progress_figure && cutter.points.size() < 3)
            ui->closeFigureButton->setDisabled(true);
    }

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
}

void MainWindow::on_addFigureButton_clicked()
{
    if (cutter.exists)
        return;
    draw_cutter = true;
    draw_figure = false;
    ui->closeFigureButton->setDisabled(true);
    ui->deleteFigureButton->setDisabled(true);
}

void MainWindow::on_backButton_clicked()
{
    if (draw_figure && progress_figure) {
        draw_figure = false;
        if (figures.back().points.size() > 2)
            ui->closeLineButton->setDisabled(true);

        figures.pop_back();
        progress_figure = false;
    } else if (draw_cutter) {
        draw_cutter = false;

        cutter.points.clear();
        progress_cutter = false;
    } else if (lastevents.empty()) {
        return;
    } else if (lastevents.back() == EVENT_FIGURE) {
        if (figures.back().points.size() > 2)
            ui->closeLineButton->setDisabled(true);
        figures.pop_back();
        draw_figure = true;

        lastevents.pop_back();
    } else if (lastevents.back() == EVENT_CUTTER) {
        cutter.exists = false;
        ui->addFigureButton->setEnabled(true);
        ui->closeFigureButton->setDisabled(true);

        cutter.points.clear();
        lastevents.pop_back();
    } else if (lastevents.back() == EVENT_DELETE_CUTTER) {
        cutter = deleted_cutters.back();
        deleted_cutters.pop_back();

        lastevents.pop_back();

        if (deleted_cutters.empty()) {
            ui->deleteFigureButton->setDisabled(true);
            ui->closeFigureButton->setDisabled(true);
        }
    }

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
}

void MainWindow::on_clearButton_clicked()
{
    lastevents.clear();
    cutter.exists = false;
    cutter.points.clear();
    deleted_cutters.clear();
    figures.clear();

    draw_cutter = false;
    draw_figure = true;

    progress_cutter = false;
    progress_figure = false;

    ui->addFigureButton->setEnabled(true);
    ui->deleteFigureButton->setDisabled(true);
    ui->closeFigureButton->setDisabled(true);

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
}

void MainWindow::on_deleteFigureButton_clicked()
{
    deleted_cutters.push_back(cutter);
    cutter.exists = false;
    cutter.points.clear();
    ui->addFigureButton->setEnabled(true);
    ui->closeFigureButton->setDisabled(true);

    lastevents.push_back(EVENT_DELETE_CUTTER);
    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
}

void MainWindow::on_closeFigureButton_clicked()
{
    if (cutter.points.size() < 3)
        return;

    cutter.exists = true;

    draw_cutter = false;
    progress_cutter = false;
    draw_figure = true;

    ui->addFigureButton->setDisabled(true);
    ui->closeFigureButton->setDisabled(true);

    lastevents.push_back(EVENT_CUTTER);

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
}

void MainWindow::on_closeLineButton_clicked()
{
    if (figures.back().points.size() < 3)
        return;

    progress_figure = false;
    draw_figure = true;
    figures.back().exists = true;

    ui->closeLineButton->setDisabled(true);

    lastevents.push_back(EVENT_FIGURE);

    draw_all(colors, draw_figure, progress_figure, draw_cutter, progress_cutter, figures, cutter, ui->graphicsView);
}
