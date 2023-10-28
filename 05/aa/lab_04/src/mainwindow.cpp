#include <QPixmap>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QTimer>
#include <QColorDialog>

#include "inc/mainwindow.h"
#include "inc/scene.h"
#include "inc/forest.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto scene = new QGraphicsScene;
    ui->canvas->setScene(scene);
    ui->canvas->scene()->clear();

    s = Scene();
}

MainWindow::~MainWindow()
{
    delete ui->canvas->scene();
    delete ui;
}

void MainWindow::showEvent(QShowEvent* ev)
{
    setLabelColor(treeColor, ui->treeColorBox);
    setLabelColor(leafColor, ui->leafColorBox);
    setLabelColor(planeColor, ui->planeColorBox);
    setLabelColor(backgroundColor, ui->backgroundColorBox);

    QMainWindow::showEvent(ev);
    QTimer::singleShot(50, this, SLOT(windowShown()));
}

void MainWindow::windowShown()
{
    setLabelColor(treeColor, ui->treeColorBox);
    setLabelColor(leafColor, ui->leafColorBox);
    setLabelColor(planeColor, ui->planeColorBox);
    setLabelColor(backgroundColor, ui->backgroundColorBox);
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

void MainWindow::on_treeColorButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(treeColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    treeColor = color;

    setLabelColor(color, ui->treeColorBox);
}


void MainWindow::on_leafColorButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(leafColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    leafColor = color;

    setLabelColor(color, ui->leafColorBox);
}


void MainWindow::on_planeColorButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(planeColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    planeColor = color;

    setLabelColor(color, ui->planeColorBox);
}


void MainWindow::on_backgroundButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(backgroundColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    backgroundColor = color;

    setLabelColor(color, ui->backgroundColorBox);
}


void MainWindow::on_generateForestButton_clicked()
{
    Forest f = Forest(
                ui->numberInput->value(),
                ui->seedInput->value(),
                QVector2D(ui->blXInput->value(), ui->blZInput->value()),
                QVector2D(ui->trXInput->value(), ui->trZInput->value()),
                ui->yInput->value(),
                treeColor,
                leafColor,
                planeColor,
                ui->ratioInput->value(),
                ui->spreadInput->value(),
                ui->splitSizeInput->value()
    );

    s.setForest(f);
}

void MainWindow::on_drawButton_clicked()
{
    s.setSize(ui->canvas->width(), ui->canvas->height());
    s.setBackground(backgroundColor);
    s.growForest(ui->growInput->value(), ui->iterationsInput->value());

    auto image = s.paint(16);

    auto pixmap = QPixmap::fromImage(*image);
    ui->canvas->scene()->addPixmap(pixmap);
}

