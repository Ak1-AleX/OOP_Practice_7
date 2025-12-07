#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), currentNumberType(0)
{
    ui->setupUi(this);
    setWindowTitle("Работа с полиномами (ПР6, клиент)");

    // Создаем комбобокс программно и добавляем в layout
    numberTypeComboBox = new QComboBox(this);
    numberTypeComboBox->addItem("Вещественные числа");
    numberTypeComboBox->addItem("Комплексные числа");
    numberTypeComboBox->setCurrentIndex(0);

    // Добавляем комбобокс в layout после titleLabel
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (mainLayout) {
        mainLayout->insertWidget(1, numberTypeComboBox); // Вставляем после titleLabel (индекс 1)
    }

    // Подключаем сигнал
    connect(numberTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_numberTypeComboBox_currentIndexChanged);

    // инициализация клиентского приложения
    clientApp = new ClientApp(this);
    connect(clientApp, &ClientApp::responseReceived, this, &MainWindow::onServerResponse);
    connect(clientApp, &ClientApp::errorOccurred, this, &MainWindow::onServerError);

    // запуск клиента - ИСПРАВЛЕНА ЛОГИЧЕСКАЯ ОШИБКА
    if (!clientApp->start()) {
        showError("Не удалось запустить клиент");
    } else {
        showInfo("Клиент запущен. Ожидание подключения к серверу...");
    }

    updateDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
    // numberTypeComboBox удалится автоматически как child widget
}

void MainWindow::updateDisplay()
{
    QString numberType = (currentNumberType == 0) ? "вещественные" : "комплексные";
    ui->statusLabel->setText("Клиентское приложение (" + numberType + " числа)");
}

void MainWindow::onServerResponse(const QString& result) {
    ui->resultTextEdit->setPlainText(result);
    showInfo("Операция выполнена успешно");
}

void MainWindow::onServerError(const QString& error) {
    showError(error);
}

void MainWindow::showError(const QString& message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void MainWindow::showInfo(const QString& message)
{
    QMessageBox::information(this, "Информация", message);
}

void MainWindow::on_numberTypeComboBox_currentIndexChanged(int index) {
    currentNumberType = index;
    updateDisplay();
    QString messageText = "Тип чисел изменен на: " + (index == 0 ? QString("вещественные") : QString("комплексные"));
    showInfo(messageText);
}

void MainWindow::on_inputPolynomButton_clicked() {
    bool ok;

    int degree = QInputDialog::getInt(this, "Ввод полинома",
                                      "Введите степень полинома:", 2, 0, 10, 1, &ok);
    if (!ok) return;

    if (degree == 0) {
        QVector<QString> roots;
        clientApp->createPolynom(currentNumberType, 0, "1", roots);
        return;
    }

    QString coeffText = QInputDialog::getText(this, "Старший коэффициент",
                                              "Введите старший коэффициент an:",
                                              QLineEdit::Normal, "1", &ok);
    if (!ok || coeffText.isEmpty()) return;

    QVector<QString> roots;
    for (int i = 0; i < degree; i++) {
        QString rootText = QInputDialog::getText(this, QString("Корень %1").arg(i+1),
                                                 QString("Введите корень %1:").arg(i+1),
                                                 QLineEdit::Normal, "0", &ok);
        if (!ok) return;
        roots.append(rootText);
    }

    clientApp->createPolynom(currentNumberType, degree, coeffText, roots);
}

void MainWindow::on_changeLeadingCoeffButton_clicked() {
    bool ok;
    QString coeffText = QInputDialog::getText(this, "Изменение старшего коэффициента",
                                              "Введите новый старший коэффициент an:",
                                              QLineEdit::Normal, "1", &ok);
    if (!ok || coeffText.isEmpty()) return;

    clientApp->changeLeadingCoeff(currentNumberType, coeffText);
}

void MainWindow::on_changeRootButton_clicked() {
    bool ok;
    int index = QInputDialog::getInt(this, "Изменение корня",
                                     "Введите индекс корня (начинается с 0):",
                                     0, 0, 10, 1, &ok);
    if (!ok) return;

    QString rootText = QInputDialog::getText(this, "Изменение корня",
                                             QString("Введите новое значение корня %1:").arg(index),
                                             QLineEdit::Normal, "0", &ok);
    if (!ok || rootText.isEmpty()) return;

    clientApp->changeRoot(currentNumberType, index, rootText);
}

void MainWindow::on_evaluateButton_clicked() {
    bool ok;
    QString pointText = QInputDialog::getText(this, "Вычисление значения",
                                              "Введите точку x для вычисления:",
                                              QLineEdit::Normal, "0", &ok);
    if (!ok || pointText.isEmpty()) return;

    clientApp->evaluate(currentNumberType, pointText);
}

void MainWindow::on_showForm1Button_clicked() {
    clientApp->getForm1(currentNumberType);
}

void MainWindow::on_showForm2Button_clicked() {
    clientApp->getForm2(currentNumberType);
}

void MainWindow::on_exitButton_clicked()
{
    close();
}
