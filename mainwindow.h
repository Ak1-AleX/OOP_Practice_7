#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "number.h"

// Включаем заголовки шаблонных классов
#include "function.h"
#include "sinfunction.h"
#include "sifunction.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calculateButton_clicked();
    void on_functionComboBox_currentIndexChanged(int index);
    void on_degreeSpinBox_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    // Текущие параметры
    int currentFunctionType;  // 0 - sin, 1 - Si
    int currentDegree;

    // Указатели на функции
    TFunction<number>* currentFunction;
    TFsin<number>* sinFunction;
    TFSi<number>* siFunction;

    void updateFunction();
    void updateDisplay();
    void showResult(const QString& result);
    void showError(const QString& message);
};

#endif // MAINWINDOW_H
