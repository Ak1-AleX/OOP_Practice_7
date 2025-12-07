#include <QCoreApplication>
#include <QDebug>
#include "serverApp.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ServerApp server;
    if (!server.start(10001)) {
        qCritical() << "Не удалось запустить сервер";
        return -1;
    }

    qInfo() << "=========================================";
    qInfo() << "Сервер полиномов запущен (ПР6)";
    qInfo() << "Порт: 10001";
    qInfo() << "Поддержка типов чисел: вещественные/комплексные";
    qInfo() << "Многопользовательский режим: ДА";
    qInfo() << "Параметризованные классы: ДА";
    qInfo() << "Ожидание запросов от клиента...";
    qInfo() << "=========================================";

    return a.exec();
}
