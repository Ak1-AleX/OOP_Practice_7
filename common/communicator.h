#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QObject>
#include "message.h"

class NetworkCommunicator : public QObject {
    Q_OBJECT

public:
    NetworkCommunicator(QObject* parent = nullptr);
    ~NetworkCommunicator();

    bool startServer(quint16 port);
    bool startClient(quint16 clientPort, quint16 serverPort = 10001);
    void sendMessage(const Message& msg, const QHostAddress& address = QHostAddress::LocalHost, quint16 port = 10001);
    bool hasPendingMessages() const;

signals:
    void messageReceived(const Message& msg);

private slots:
    void onReadyRead();

private:
    QUdpSocket* socket;
    QHostAddress serverAddress;
    quint16 serverPort;
    quint16 clientPort;
};

#endif // COMMUNICATOR_H
