#include "message.h"
#include <QDataStream>

QByteArray Message::serialize() const {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << *this;
    return data;
}

Message Message::deserialize(const QByteArray& data) {
    Message msg;
    QDataStream stream(data);
    stream >> msg;
    return msg;
}

Message Message::createPolynom(int numberType, int degree, const QString& leadingCoeff, const QVector<QString>& roots) {
    QVector<QString> params;
    params.append(QString::number(numberType));
    params.append(QString::number(degree));
    params.append(leadingCoeff);
    for (const QString& root : roots) {
        params.append(root);
    }
    return Message(MessageType::CREATE_POLYNOM, "Create polynom", params);
}

Message Message::changeLeadingCoeff(int numberType, const QString& newCoeff) {
    QVector<QString> params;
    params.append(QString::number(numberType));
    params.append(newCoeff);
    return Message(MessageType::CHANGE_LEADING_COEFF, "Change leading coefficient", params);
}

Message Message::changeRoot(int numberType, int index, const QString& newRoot) {
    QVector<QString> params;
    params.append(QString::number(numberType));
    params.append(QString::number(index));
    params.append(newRoot);
    return Message(MessageType::CHANGE_ROOT, "Change root", params);
}

Message Message::evaluate(int numberType, const QString& point) {
    QVector<QString> params;
    params.append(QString::number(numberType));
    params.append(point);
    return Message(MessageType::EVALUATE, "Evaluate polynom", params);
}

Message Message::getForm1(int numberType) {
    QVector<QString> params;
    params.append(QString::number(numberType));
    return Message(MessageType::GET_FORM1, "Get coefficients form", params);
}

Message Message::getForm2(int numberType) {
    QVector<QString> params;
    params.append(QString::number(numberType));
    return Message(MessageType::GET_FORM2, "Get roots form", params);
}

Message Message::success(const QString& result) {
    return Message(MessageType::SUCCESS_RESPONSE, result);
}

Message Message::error(const QString& errorMessage) {
    return Message(MessageType::ERROR_RESPONSE, errorMessage);
}

QDataStream& operator<<(QDataStream& out, const Message& msg) {
    out << static_cast<qint32>(msg.type);
    out << msg.data;
    out << msg.params;
    return out;
}

QDataStream& operator>>(QDataStream& in, Message& msg) {
    qint32 type;
    in >> type;
    msg.type = static_cast<MessageType>(type);
    in >> msg.data;
    in >> msg.params;
    return in;
}
