#include <QColorDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QWheelEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QGraphicsScene* scene = new QGraphicsScene();
    ui->canvas->setScene(scene);
    ui->canvas->scene()->clear();
}

MainWindow::~MainWindow() {
    delete ui->canvas->scene();

    delete ui;
}

void MainWindow::showEvent(QShowEvent* e) {
    QMainWindow::showEvent(e);

    setLabelColor(bgColor, ui->bgColorBox);
    setLabelColor(lineColor, ui->lineColorBox);
}

void MainWindow::resizeEvent(QResizeEvent* e) {
    QMainWindow::resizeEvent(e);
    draw();
}

void MainWindow::wheelEvent(QWheelEvent* e)
{
    ui->canvas->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scale_factor = 1.10;
    double rescale_factor = 1 / scale_factor;

    if (e->angleDelta().y() > 0) {
        ui->canvas->scale(scale_factor, scale_factor);
        numScaled++;
    } else if (numScaled > 0) {
        ui->canvas->scale(rescale_factor, rescale_factor);
        numScaled--;
    }
}

void setLabelColor(QColor color, QLabel* label)
{
    QImage image = QImage(label->geometry().width(), label->geometry().height(),
        QImage::Format_RGB32);
    QPainter p(&image);

    p.setBrush(QColor(color));
    p.setPen(QPen(color, 1));
    QRect rect = QRect(0, 0, label->geometry().width(), label->geometry().height());
    p.drawRect(rect);

    QPixmap pixmap = QPixmap::fromImage(image);
    label->clear();
    label->setPixmap(pixmap);
}

void MainWindow::on_bgColorButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(bgColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    bgColor = color ;
    setLabelColor(color, ui->bgColorBox);
    draw();
}

void MainWindow::on_lineColorButton_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(lineColor);
    dialog.show();
    dialog.exec();

    QColor color = dialog.selectedColor();
    if (!color.isValid())
        return;

    lineColor = color;
    setLabelColor(color, ui->lineColorBox);
}

void MainWindow::draw() {
    QImage image = QImage(ui->canvas->geometry().width(), ui->canvas->geometry().height(), QImage::Format_RGB32);
    QPainter p(&image);
    image.fill(bgColor);

    drawObjects(p, objects);

    QPixmap pixmap = QPixmap::fromImage(image);
    ui->canvas->scene()->clear();
    ui->canvas->scene()->addPixmap(pixmap);
}


void MainWindow::on_addLineButton_clicked() {
    bool validXStart, validYStart;
    bool validXEnd, validYEnd;

    int startX = ui->lineXStartInput->text().toInt(&validXStart);
    int startY = ui->lineYStartInput->text().toInt(&validYStart);

    int endX = ui->lineXEndInput->text().toInt(&validXEnd);
    int endY = ui->lineYEndInput->text().toInt(&validYEnd);

    if (!validXStart || !validYStart || !validXEnd || !validYEnd) {
        QMessageBox::warning(this, "Ошибка ввода", "Координаты введены неверно.");

        ui->lineXStartInput->clear();
        ui->lineYStartInput->clear();

        ui->lineXEndInput->clear();
        ui->lineYEndInput->clear();

        return;
    }

    Method method = (Method) ui->methodBox->currentIndex();

    if (method < 0) {
        QMessageBox::warning(this, "Ошибка ввода", "Неверно выбран метод.");

        return;
    }

    Line line = {
        QPoint(startX, startY),
        QPoint(endX, endY),
        method,
        lineColor,
    };

    Object object;
    object.type = LINE;
    object.line = line;

    objects.push_back(object);

    draw();
}


void MainWindow::on_addSpectreButton_clicked() {
    bool validXCenter, validYCenter;
    bool validAngle, validRadius;

    int centerX = ui->spectreXCentreInput->text().toInt(&validXCenter);
    int centerY = ui->spectreYCentreInput->text().toInt(&validYCenter);

    double angle = ui->spectreAngleInput->text().toDouble(&validAngle);
    double radius = ui->spectreRadiusInput->text().toDouble(&validRadius);

    if (!validXCenter || !validYCenter) {
        QMessageBox::warning(this, "Ошибка ввода", "Координаты введены неверно.");

        ui->spectreXCentreInput->clear();
        ui->spectreYCentreInput->clear();

        ui->spectreAngleInput->clear();
        ui->spectreRadiusInput->clear();

        return;
    }

    if (!validAngle || !validRadius || angle <= 0.0 || angle >= 10.0 || radius <= 0) {
        QMessageBox::warning(this, "Ошибка ввода", "Значения введены неверно.");

        ui->spectreXCentreInput->clear();
        ui->spectreYCentreInput->clear();

        ui->spectreAngleInput->clear();
        ui->spectreRadiusInput->clear();

        return;
    }

    Method method = (Method) ui->methodBox->currentIndex();

    if (method < 0) {
        QMessageBox::warning(this, "Ошибка ввода", "Неверно выбран метод.");

        return;
    }

    Spectre spectre = {
        QPoint(centerX, centerY),

        radius,
        angle,

        method,
        lineColor,
    };

    Object object;
    object.type = SPECTRE;
    object.spectre = spectre;

    objects.push_back(object);

    draw();
}

void MainWindow::on_cancelButton_clicked() {
    if (!objects.empty())
        objects.pop_back();

    draw();
}

void MainWindow::on_clearButton_clicked() {
    objects.clear();
    draw();
}

void MainWindow::on_showTimeButton_clicked() {
    bool validXCenter, validYCenter;
    bool validAngle, validRadius;

    int centerX = ui->spectreXCentreInput->text().toInt(&validXCenter);
    int centerY = ui->spectreYCentreInput->text().toInt(&validYCenter);

    double angle = ui->spectreAngleInput->text().toDouble(&validAngle);
    double radius = ui->spectreRadiusInput->text().toDouble(&validRadius);

    if (!validXCenter || !validYCenter) {
        QMessageBox::warning(this, "Ошибка ввода", "Координаты введены неверно.");

        ui->spectreXCentreInput->clear();
        ui->spectreYCentreInput->clear();

        ui->spectreAngleInput->clear();
        ui->spectreRadiusInput->clear();

        return;
    }

    if (!validAngle || !validRadius || angle <= 0.0 || angle >= 10.0 || radius <= 0) {
        QMessageBox::warning(this, "Ошибка ввода", "Значения введены неверно.");

        ui->spectreXCentreInput->clear();
        ui->spectreYCentreInput->clear();

        ui->spectreAngleInput->clear();
        ui->spectreRadiusInput->clear();

        return;
    }

    Method method = (Method) 0;

    Spectre spectre = {
        QPoint(centerX, centerY),

        radius,
        angle,

        method,
        lineColor,
    };

    checkTime(spectre);
}


void MainWindow::on_showStepsButton_clicked() {
    bool validXCenter, validYCenter;
    bool validAngle, validRadius;

    int centerX = ui->spectreXCentreInput->text().toInt(&validXCenter);
    int centerY = ui->spectreYCentreInput->text().toInt(&validYCenter);

    double angle = ui->spectreAngleInput->text().toDouble(&validAngle);
    double radius = ui->spectreRadiusInput->text().toDouble(&validRadius);

    if (!validXCenter || !validYCenter) {
        QMessageBox::warning(this, "Ошибка ввода", "Координаты введены неверно.");

        ui->spectreXCentreInput->clear();
        ui->spectreYCentreInput->clear();

        ui->spectreAngleInput->clear();
        ui->spectreRadiusInput->clear();

        return;
    }

    if (!validAngle || !validRadius || angle <= 0.0 || angle >= 10.0 || radius <= 0) {
        QMessageBox::warning(this, "Ошибка ввода", "Значения введены неверно.");

        ui->spectreXCentreInput->clear();
        ui->spectreYCentreInput->clear();

        ui->spectreAngleInput->clear();
        ui->spectreRadiusInput->clear();

        return;
    }

    Method method = (Method) 0;

    Spectre spectre = {
        QPoint(centerX, centerY),

        radius,
        angle,

        method,
        lineColor,
    };

    checkSteps(spectre);
}

void MainWindow::on_exitAction_triggered() {
    QApplication::quit();
}

void MainWindow::on_programInfoAction_triggered() {
    QMessageBox::about(this, "О программе", "Программа для всяких прямых.\n");
}

void MainWindow::on_authorInfoAction_triggered() {
    QMessageBox box(this);
    box.setWindowTitle("Об авторе");
    box.setTextFormat(Qt::RichText);
    box.setText("Алеша Княжев, ИУ7-42Б.<br><br> <a href='https://github.com/muhomorfus'>GitHub</a>");
    box.exec();
}
