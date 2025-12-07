#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QVector>
#include <QDataStream>

enum class MessageType {
    CREATE_POLYNOM,
    CHANGE_LEADING_COEFF,
    CHANGE_ROOT,
    EVALUATE,
    GET_FORM1,
    GET_FORM2,
    SUCCESS_RESPONSE,
    ERROR_RESPONSE
};

struct Message {
    MessageType type;
    QString data;
    QVector<QString> params;

    Message() = default;
    Message(MessageType t, const QString& d = "", const QVector<QString>& p = {})
        : type(t), data(d), params(p) {}

    QByteArray serialize() const;
    static Message deserialize(const QByteArray& data);

    static Message createPolynom(int numberType, int degree, const QString& leadingCoeff, const QVector<QString>& roots);
    static Message changeLeadingCoeff(int numberType, const QString& newCoeff);
    static Message changeRoot(int numberType, int index, const QString& newRoot);
    static Message evaluate(int numberType, const QString& point);
    static Message getForm1(int numberType);
    static Message getForm2(int numberType);
    static Message success(const QString& result);
    static Message error(const QString& errorMessage);
};

QDataStream& operator<<(QDataStream& out, const Message& msg);
QDataStream& operator>>(QDataStream& in, Message& msg);

#endif // MESSAGE_H
