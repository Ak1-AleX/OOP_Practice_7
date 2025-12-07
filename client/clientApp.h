#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include <QObject>
#include <QTimer>
#include "../common/communicator.h"
#include "../common/message.h"

class ClientApp : public QObject {
    Q_OBJECT

public:
    explicit ClientApp(QObject* parent = nullptr);
    ~ClientApp();

    bool start(quint16 clientPort = 10000, quint16 serverPort = 10001);

    void createPolynom(int numberType, int degree, const QString& leadingCoeff, const QVector<QString>& roots);
    void changeLeadingCoeff(int numberType, const QString& newCoeff);
    void changeRoot(int numberType, int index, const QString& newRoot);
    void evaluate(int numberType, const QString& point);
    void getForm1(int numberType);
    void getForm2(int numberType);

signals:
    void responseReceived(const QString& result);
    void errorOccurred(const QString& error);

private slots:
    void handleServerResponse(const Message& msg);
    void checkForResponse();

private:
    NetworkCommunicator* communicator;
    QTimer* responseTimer;
    bool waitingForResponse;

    void sendRequest(const Message& request);
    void processSuccessResponse(const Message& msg);
    void processErrorResponse(const Message& msg);
};

#endif // CLIENTAPP_H
