#ifndef SERVERAPP_H
#define SERVERAPP_H

#include <QObject>
#include <QCoreApplication>
#include <QMap>
#include <QHostAddress>
#include "polynom.h"
#include "complex.h"
#include "../common/communicator.h"
#include "../common/message.h"

class ServerApp : public QObject {
    Q_OBJECT

public:
    ServerApp(QObject* parent = nullptr);
    bool start(quint16 port = 10001);

private slots:
    void handleMessage(const Message& msg);

private:
    NetworkCommunicator* communicator;
    QMap<QString, Polynom<TComplex>> clientPolynoms;

    QString generateClientKey(const QHostAddress& address, quint16 port);
    Polynom<TComplex> createPolynomForClient(const Message& msg);

    void processCreatePolynom(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort);
    void processChangeLeadingCoeff(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort);
    void processChangeRoot(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort);
    void processEvaluate(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort);
    void processGetForm1(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort);
    void processGetForm2(const Message& msg, const QHostAddress& clientAddress, quint16 clientPort);

    void sendResponse(const Message& response, const QHostAddress& address = QHostAddress::LocalHost, quint16 port = 10000);
};

#endif // SERVERAPP_H
