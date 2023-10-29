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
#include "task.h"
#include "benches.h"
#include "errors.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene();
    ui->canvas->setScene(scene);
    ui->canvas->scene()->clear();

    connect(ui->typeBox, SIGNAL(currentIndexChanged(int)),
            SLOT(typeBoxIndexChanged(int)));
    connect(ui->bgColorButton, SIGNAL(clicked()), SLOT(bgColorButtonClicked()));
    connect(ui->lineColorButton, SIGNAL(clicked()),
            SLOT(lineColorButtonClicked()));

    connect(ui->drawButton, SIGNAL(clicked()), SLOT(drawButtonClicked()));
    connect(ui->timeButton, SIGNAL(clicked()), SLOT(timeButtonClicked()));

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

    drawAll();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::Wheel && object == ui->canvas->viewport()) {
        QWheelEvent *we = static_cast<QWheelEvent *>(event);
        wheelEvent(we);
        return true;
    }
    return false;
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    ui->canvas->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.1;
    double unscaleFactor = 1 / scaleFactor;

    if (event->angleDelta().y() > 0) {
        ui->canvas->scale(scaleFactor, scaleFactor);
        scaled++;
    } else if (scaled > 0) {
        ui->canvas->scale(unscaleFactor, unscaleFactor);
        scaled--;
    }
}

void MainWindow::windowShown() {
    setLabelColor(bgColor, ui->bgColorLabel);
    setLabelColor(lineColor, ui->lineColorLabel);
    typeBoxIndexChanged(0);
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

Error MainWindow::readObject(Object &object) {
    bool validX, validY;
    bool validRa, validRb;
    bool validDRa, validDRb;
    bool validN;

    int x = ui->xInput->text().toInt(&validX);
    int y = ui->yInput->text().toInt(&validY);
    int ra = ui->raInput->text().toInt(&validRa);
    int rb = ui->rbInput->text().toInt(&validRb);
    int dRa = ui->dRaInput->text().toInt(&validDRa);
    int dRb = ui->dRbInput->text().toInt(&validDRb);
    int n = ui->nInput->text().toInt(&validN);

    if (!validX || !validY) {
        return INVALID_COORDINATES_ERROR;
    }

    Circle circle = {
        .center = QPoint(x, y),
        .r = ra,
    };

    Ellipse ellipse = {
        .center = QPoint(x, y),
        .ra = ra,
        .rb = rb,
    };

    object.method = Method(ui->methodBox->currentIndex());
    object.color = lineColor;

    if (ui->typeBox->currentIndex() == 0) {
        if (!validRa) {
            return INVALID_RADIUS_ERROR;
        }

        object.type = CIRCLE;
        object.circle = circle;
    } else if (ui->typeBox->currentIndex() == 1) {
        if (!validRa || !validRb) {
            return INVALID_RADIUS_ERROR;
        }

        object.type = ELLIPSE;
        object.ellipse = ellipse;
    } else if (ui->typeBox->currentIndex() == 2) {
        if (!validRa || !validDRa) {
            return INVALID_STEP_ERROR;
        }

        CircleSpectre spectre = {
            .circle = circle,
            .dR = dRa,
            .n = n,
        };

        object.type = CIRCLE_SPECTRE;
        object.circleSpectre = spectre;
    } else if (ui->typeBox->currentIndex() == 3) {
        if (!validRa || !validRb) {
            return INVALID_RADIUS_ERROR;
        }

        if (!validDRa || !validDRb) {
            return INVALID_STEP_ERROR;
        }

        EllipseSpectre spectre = {
            .ellipse = ellipse,
            .dRa = dRa,
            .dRb = dRb,
            .n = n,
        };

        object.type = ELLIPSE_SPECTRE;
        object.ellipseSpectre = spectre;
    } else {
        return UNKNOWN_ERROR;
    }

    return OK;
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
    drawAll();
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

void MainWindow::typeBoxIndexChanged(int index) {
    if (index == 0) {
        ui->rbInput->setDisabled(true);
        ui->dRaInput->setDisabled(true);
        ui->dRbInput->setDisabled(true);
        ui->nInput->setDisabled(true);
        ui->timeButton->setDisabled(true);
    } else if (index == 1) {
        ui->rbInput->setDisabled(false);
        ui->dRaInput->setDisabled(true);
        ui->dRbInput->setDisabled(true);
        ui->nInput->setDisabled(true);
        ui->timeButton->setDisabled(true);
    } else if (index == 2) {
        ui->rbInput->setDisabled(true);
        ui->dRaInput->setDisabled(false);
        ui->dRbInput->setDisabled(true);
        ui->nInput->setDisabled(false);
        ui->timeButton->setDisabled(false);
    } else {
        ui->rbInput->setDisabled(false);
        ui->dRaInput->setDisabled(false);
        ui->dRbInput->setDisabled(false);
        ui->nInput->setDisabled(false);
        ui->timeButton->setDisabled(false);
    }
}

void MainWindow::drawAll() {
    QImage image = QImage(ui->canvas->geometry().width(), ui->canvas->geometry().height(), QImage::Format_RGB32);
    image.fill(bgColor);

    QPixmap pixmap = drawObjects(objects, image);

    ui->canvas->scene()->clear();
    ui->canvas->scene()->addPixmap(pixmap);
}

void MainWindow::drawButtonClicked() {
    Object object;

    bool ok = processError(readObject(object));
    if (!ok) {
        return;
    }

    objects.push_back(object);
    drawAll();
}

void MainWindow::undoButtonClicked() {
    if (objects.empty()) {
        return;
    }

    objects.pop_back();
    drawAll();
}

void MainWindow::clearButtonClicked() {
    objects.clear();
    drawAll();
}

void MainWindow::timeButtonClicked() {
    Object object;

    bool ok = processError(readObject(object));
    if (!ok) {
        return;
    }

    bench(object, "time.txt");
    system("python3 time.py");
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
