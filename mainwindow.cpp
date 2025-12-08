#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "complex.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentFunctionType(0)
    , currentDegree(5)
    , currentFunction(nullptr)
    , sinFunction(nullptr)
    , siFunction(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Вычисление функций через полиномы (ПР7)");

    // Настройка интерфейса
    ui->degreeSpinBox->setRange(1, 20);
    ui->degreeSpinBox->setValue(currentDegree);

    ui->functionComboBox->addItem("sin(x)");
    ui->functionComboBox->addItem("Si(x) = sin(x)/x");

    // Инициализация функций
    updateFunction();
    updateDisplay();
}

MainWindow::~MainWindow()
{
    delete sinFunction;
    delete siFunction;
    delete ui;
}

void MainWindow::updateFunction()
{
    // Удаляем старые функции
    delete sinFunction;
    delete siFunction;

    // Создаем новые функции с текущей степенью
    sinFunction = new TFsin<number>(currentDegree);
    siFunction = new TFSi<number>(currentDegree);

    // Устанавливаем текущую функцию
    if (currentFunctionType == 0) {
        currentFunction = sinFunction;
    } else {
        currentFunction = siFunction;
    }
}

void MainWindow::updateDisplay()
{
    QString functionName = (currentFunctionType == 0) ? "sin(x)" : "Si(x) = sin(x)/x";
    ui->statusLabel->setText(QString("Функция: %1, Степень полинома: %2")
                                 .arg(functionName).arg(currentDegree));

    // Отображаем полином
    if (currentFunction) {
        ui->polynomTextEdit->setPlainText(currentFunction->toString());
    } else {
        ui->polynomTextEdit->clear();
    }
}

void MainWindow::on_calculateButton_clicked()
{
    QString pointText = ui->pointLineEdit->text().trimmed();

    if (pointText.isEmpty()) {
        showError("Введите точку для вычисления");
        return;
    }

    if (!currentFunction) {
        showError("Функция не инициализирована");
        return;
    }

    try {
        // Преобразуем строку в комплексное число
        TComplex x = TComplex::fromString(pointText);

        // Вычисляем значение функции через текущую функцию
        number result = currentFunction->value(x);

        // Выводим результат
        QString functionName = (currentFunctionType == 0) ? "sin" : "Si";
        QString resultText = QString("%1(%2) = %3\n\n")
                                 .arg(functionName)
                                 .arg(pointText)
                                 .arg(result.toString());

        // Добавляем информацию о полиноме
        resultText += QString("Полином степени %1 (ряд Маклорена):\n%2")
                          .arg(currentDegree)
                          .arg(currentFunction->toString());

        showResult(resultText);

    } catch (const std::exception& e) {
        showError(QString("Ошибка вычисления: %1").arg(e.what()));
    } catch (...) {
        showError("Неизвестная ошибка при вычислении");
    }
}

void MainWindow::on_functionComboBox_currentIndexChanged(int index)
{
    currentFunctionType = index;
    updateFunction();
    updateDisplay();
}

void MainWindow::on_degreeSpinBox_valueChanged(int value)
{
    currentDegree = value;
    updateFunction();
    updateDisplay();
}

void MainWindow::showResult(const QString& result)
{
    ui->resultTextEdit->setPlainText(result);
    QMessageBox::information(this, "Результат", "Вычисление выполнено успешно");
}

void MainWindow::showError(const QString& message)
{
    QMessageBox::critical(this, "Ошибка", message);
    ui->resultTextEdit->setPlainText("ОШИБКА: " + message);
}
