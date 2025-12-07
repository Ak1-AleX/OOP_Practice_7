#include "serverApp.h"
#include <QDebug>
#include <QHostAddress>
#include <stdexcept>

ServerApp::ServerApp(QObject* parent) : QObject(parent) {
    communicator = new NetworkCommunicator(this);
    connect(communicator, &NetworkCommunicator::messageReceived, this, &ServerApp::handleMessage);
}

bool ServerApp::start(quint16 port) {
    return communicator->startServer(port);
}

QString ServerApp::generateClientKey(const QHostAddress& address, quint16 port) {
    return address.toString() + ":" + QString::number(port);
}

Polynom<TComplex> ServerApp::createPolynomForClient(const Message& msg) {
    Polynom<TComplex> polynom;

    if (msg.params.size() < 3) {
        throw std::invalid_argument("Недостаточно параметров для создания полинома");
    }

    int numberType = msg.params[0].toInt();
    int degree = msg.params[1].toInt();
    QString leadingCoeffStr = msg.params[2];

    QVector<QString> roots;
    for (int i = 3; i < msg.params.size(); i++) {
        roots.append(msg.params[i]);
    }

    if (roots.size() != degree) {
        throw std::invalid_argument("Количество корней не соответствует степени полинома");
    }

    QVector<TComplex> numberRoots;
    for (const QString& rootStr : roots) {
        numberRoots.append(TComplex::fromString(rootStr));
    }

    TComplex leadingCoeff = TComplex::fromString(leadingCoeffStr);
    polynom.setFromRoots(degree, leadingCoeff, numberRoots);

    return polynom;
}

void ServerApp::handleMessage(const Message& msg) {
    QHostAddress clientAddress = QHostAddress::LocalHost;
    quint16 clientPort = 10000;

    qDebug() << "Получено сообщение от" << clientAddress.toString() << "порт" << clientPort;

    switch (msg.type) {
    case MessageType::CREATE_POLYNOM:
        processCreatePolynom(msg, clientAddress, clientPort);
        break;
    case MessageType::CHANGE_LEADING_COEFF:
        processChangeLeadingCoeff(msg, clientAddress, clientPort);
        break;
    case MessageType::CHANGE_ROOT:
        processChangeRoot(msg, clientAddress, clientPort);
        break;
    case MessageType::EVALUATE:
        processEvaluate(msg, clientAddress, clientPort);
        break;
    case MessageType::GET_FORM1:
        processGetForm1(msg, clientAddress, clientPort);
        break;
    case MessageType::GET_FORM2:
        processGetForm2(msg, clientAddress, clientPort);
        break;
    default:
        qDebug() << "Неизвестный тип сообщения";
        sendResponse(Message::error("Неизвестный тип сообщения"), clientAddress, clientPort);
        break;
    }
}

void ServerApp::processCreatePolynom(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort) {
    try {
        Polynom<TComplex> polynom = createPolynomForClient(msg);

        QString clientKey = generateClientKey(clientAddress, clientPort);
        clientPolynoms[clientKey] = polynom;

        int numberType = msg.params[0].toInt();
        QString numberTypeStr = (numberType == 0) ? "вещественные" : "комплексные";

        QString result = "Полином создан (" + numberTypeStr + " числа):\n\n" +
                         polynom.toStringForm1() + "\n\n" +
                         polynom.toStringForm2();

        sendResponse(Message::success(result), clientAddress, clientPort);

    } catch (const std::exception& e) {
        sendResponse(Message::error("Ошибка создания полинома: " + QString(e.what())), clientAddress, clientPort);
    }
}

void ServerApp::processChangeLeadingCoeff(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort) {
    QString clientKey = generateClientKey(clientAddress, clientPort);

    if (!clientPolynoms.contains(clientKey)) {
        sendResponse(Message::error("Полином не задан. Создайте полином сначала."), clientAddress, clientPort);
        return;
    }

    try {
        if (msg.params.size() < 2) {
            sendResponse(Message::error("Не указан новый коэффициент"), clientAddress, clientPort);
            return;
        }

        int numberType = msg.params[0].toInt();
        QString coeffStr = msg.params[1];
        TComplex newCoeff = TComplex::fromString(coeffStr);

        Polynom<TComplex>& polynom = clientPolynoms[clientKey];
        polynom.setLeadingCoeff(newCoeff);

        QString numberTypeStr = (numberType == 0) ? "вещественные" : "комплексные";
        QString result = "Коэффициент изменен (" + numberTypeStr + " числа):\n\n" + polynom.toStringForm1();
        sendResponse(Message::success(result), clientAddress, clientPort);

    } catch (const std::exception& e) {
        sendResponse(Message::error("Ошибка изменения коэффициента: " + QString(e.what())), clientAddress, clientPort);
    }
}

void ServerApp::processChangeRoot(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort) {
    QString clientKey = generateClientKey(clientAddress, clientPort);

    if (!clientPolynoms.contains(clientKey)) {
        sendResponse(Message::error("Полином не задан. Создайте полином сначала."), clientAddress, clientPort);
        return;
    }

    try {
        if (msg.params.size() < 3) {
            sendResponse(Message::error("Недостаточно параметров для изменения корня"), clientAddress, clientPort);
            return;
        }

        int numberType = msg.params[0].toInt();
        int index = msg.params[1].toInt();
        QString newRootStr = msg.params[2];
        TComplex newRoot = TComplex::fromString(newRootStr);

        Polynom<TComplex>& polynom = clientPolynoms[clientKey];

        if (index < 0 || index >= polynom.getDegree()) {
            sendResponse(Message::error("Неверный индекс корня"), clientAddress, clientPort);
            return;
        }

        polynom.setRoot(index, newRoot);

        QString numberTypeStr = (numberType == 0) ? "вещественные" : "комплексные";
        QString result = "Корень изменен (" + numberTypeStr + " числа):\n\n" + polynom.toStringForm2();
        sendResponse(Message::success(result), clientAddress, clientPort);

    } catch (const std::exception& e) {
        sendResponse(Message::error("Ошибка изменения корня: " + QString(e.what())), clientAddress, clientPort);
    }
}

void ServerApp::processEvaluate(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort) {
    QString clientKey = generateClientKey(clientAddress, clientPort);

    if (!clientPolynoms.contains(clientKey)) {
        sendResponse(Message::error("Полином не задан. Создайте полином сначала."), clientAddress, clientPort);
        return;
    }

    try {
        if (msg.params.size() < 2) {
            sendResponse(Message::error("Не указана точка вычисления"), clientAddress, clientPort);
            return;
        }

        int numberType = msg.params[0].toInt();
        QString pointStr = msg.params[1];
        TComplex x = TComplex::fromString(pointStr);

        const Polynom<TComplex>& polynom = clientPolynoms[clientKey];
        TComplex result = polynom.evaluate(x);

        QString numberTypeStr = (numberType == 0) ? "вещественные" : "комплексные";
        sendResponse(Message::success("P(" + pointStr + ") = " + result.toString() + " (" + numberTypeStr + " числа)"),
                     clientAddress, clientPort);

    } catch (const std::exception& e) {
        sendResponse(Message::error("Ошибка вычисления: " + QString(e.what())), clientAddress, clientPort);
    }
}

void ServerApp::processGetForm1(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort) {
    QString clientKey = generateClientKey(clientAddress, clientPort);

    if (!clientPolynoms.contains(clientKey)) {
        sendResponse(Message::error("Полином не задан. Создайте полином сначала."), clientAddress, clientPort);
        return;
    }

    int numberType = msg.params[0].toInt();
    const Polynom<TComplex>& polynom = clientPolynoms[clientKey];
    QString numberTypeStr = (numberType == 0) ? "вещественные" : "комплексные";
    QString result = "Форма 1 (" + numberTypeStr + " числа):\n\n" + polynom.toStringForm1();
    sendResponse(Message::success(result), clientAddress, clientPort);
}

void ServerApp::processGetForm2(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort) {
    QString clientKey = generateClientKey(clientAddress, clientPort);

    if (!clientPolynoms.contains(clientKey)) {
        sendResponse(Message::error("Полином не задан. Создайте полином сначала."), clientAddress, clientPort);
        return;
    }

    int numberType = msg.params[0].toInt();
    const Polynom<TComplex>& polynom = clientPolynoms[clientKey];
    QString numberTypeStr = (numberType == 0) ? "вещественные" : "комплексные";
    QString result = "Форма 2 (" + numberTypeStr + " числа):\n\n" + polynom.toStringForm2();
    sendResponse(Message::success(result), clientAddress, clientPort);
}

void ServerApp::sendResponse(const Message& response, const QHostAddress& address, quint16 port) {
    communicator->sendMessage(response, address, port);
}
