#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    qDebug() << "=========================================";
    qDebug() << "Клиентское приложение запущено";
    qDebug() << "Порт клиента: 10000";
    qDebug() << "Серверный порт: 10001";
    qDebug() << "Тип чисел: вещественные/комплексные";
    qDebug() << "=========================================";

    return a.exec();
}
