#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    qDebug() << "=========================================";
    qDebug() << "Приложение для вычисления функций (ПР7)";
    qDebug() << "Поддерживаемые функции: sin(x), Si(x)=sin(x)/x";
    qDebug() << "Тип чисел: комплексные";
    qDebug() << "Метод: полиномиальное приближение (ряд Маклорена)";
    qDebug() << "=========================================";

    return a.exec();
}
