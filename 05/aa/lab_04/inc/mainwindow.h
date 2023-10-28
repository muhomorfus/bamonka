/**
 * @file
 * @brief Заголовочный файл для класса MainWindow
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVector2D>

#include "inc/scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Класс главного окна приложения
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор по умолчанию
     * @param [in] parent Предок
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор
     **/
    ~MainWindow();

    /**
     * @brief Событие, вызываемое на показе окна
     * @param [in] ev Событие
     */
    void showEvent(QShowEvent* ev);

    /**
     * @brief Функция, вызываемая после открытия окна
     */
    void windowShown();

private slots:

    void on_treeColorButton_clicked();

    void on_leafColorButton_clicked();

    void on_planeColorButton_clicked();

    void on_backgroundButton_clicked();

    void on_generateForestButton_clicked();

    void on_drawButton_clicked();

private:
    Ui::MainWindow *ui;

    Scene s;

    QColor treeColor = QColor(159,140,103);
    QColor leafColor = Qt::darkRed;
    QColor planeColor = QColor(149,190,108);
    QColor backgroundColor = QColor(229,234,206);

    void setLabelColor(QColor color, QLabel* label);
};
#endif // MAINWINDOW_H
