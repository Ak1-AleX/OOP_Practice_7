#include "communicator.h"
#include <QDebug>

NetworkCommunicator::NetworkCommunicator(QObject* parent)
    : QObject(parent), socket(nullptr), serverPort(10001), clientPort(10000) {
    socket = new QUdpSocket(this);
}

NetworkCommunicator::~NetworkCommunicator() {
    if (socket) {
        socket->close();
    }
}

bool NetworkCommunicator::startServer(quint16 port) {
    if (socket->bind(QHostAddress::LocalHost, port)) {
        serverPort = port;
        connect(socket, &QUdpSocket::readyRead, this, &NetworkCommunicator::onReadyRead);
        qDebug() << "Сервер запущен на порту" << port;
        return true;
    }
    qDebug() << "Ошибка запуска сервера на порту" << port;
    return false;
}

bool NetworkCommunicator::startClient(quint16 clientPort, quint16 serverPort) {
    if (socket->bind(QHostAddress::LocalHost, clientPort)) {
        this->clientPort = clientPort;
        this->serverPort = serverPort;
        serverAddress = QHostAddress::LocalHost;
        connect(socket, &QUdpSocket::readyRead, this, &NetworkCommunicator::onReadyRead);
        qDebug() << "Клиент запущен на порту" << clientPort;
        return true;
    }
    return false;
}

void NetworkCommunicator::sendMessage(const Message& msg, const QHostAddress& address, quint16 port) {
    QByteArray data = msg.serialize();
    qint64 result = socket->writeDatagram(data, address, port);
    if (result == -1) {
        qDebug() << "Ошибка отправки сообщения:" << socket->errorString();
    } else {
        qDebug() << "Сообщение отправлено на порт" << port;
    }
}

bool NetworkCommunicator::hasPendingMessages() const {
    return socket->hasPendingDatagrams();
}

void NetworkCommunicator::onReadyRead() {
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        Message msg = Message::deserialize(datagram);
        qDebug() << "Получено сообщение от" << sender.toString() << "порт" << senderPort;
        emit messageReceived(msg);
    }
}
