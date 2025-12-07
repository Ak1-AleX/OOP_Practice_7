#include "complex.h"
#include <cmath>
#include <QString>
#include <QDebug>

TComplex::TComplex() : re(0), im(0) {}
TComplex::TComplex(double r, double i) : re(r), im(i) {}

TComplex TComplex::operator+(const TComplex& other) const {
    return TComplex(re + other.re, im + other.im);
}

TComplex TComplex::operator-(const TComplex& other) const {
    return TComplex(re - other.re, im - other.im);
}

TComplex TComplex::operator*(const TComplex& other) const {
    return TComplex(re * other.re - im * other.im,
                    re * other.im + im * other.re);
}

TComplex TComplex::operator/(const TComplex& other) const {
    double denom = other.re * other.re + other.im * other.im;
    if (denom == 0.0) {
        qDebug() << "Ошибка: деление на ноль!";
        return TComplex(0, 0);
    }
    return TComplex((re * other.re + im * other.im) / denom,
                    (im * other.re - re * other.im) / denom);
}

bool TComplex::operator==(const TComplex& other) const {
    return re == other.re && im == other.im;
}

bool TComplex::operator!=(const TComplex& other) const {
    return !(*this == other);
}

double TComplex::abs() const {
    return std::sqrt(re * re + im * im);
}

int TComplex::compare(const TComplex& other) const {
    double r1 = abs();
    double r2 = other.abs();
    if (r1 < r2) return -1;
    if (r1 > r2) return 1;

    double phi1 = atan2(im, re);
    double phi2 = atan2(other.im, other.re);
    if (phi1 < phi2) return -1;
    if (phi1 > phi2) return 1;

    return 0;
}

QString TComplex::toString() const {
    if (im == 0) return QString::number(re);
    else if (re == 0) {
        if (im == 1) return "i";
        else if (im == -1) return "-i";
        else return QString::number(im) + "i";
    }
    else {
        QString sign = (im >= 0) ? "+" : "";
        QString imagPart;
        if (im == 1) imagPart = "i";
        else if (im == -1) imagPart = "-i";
        else imagPart = QString("%1i").arg(im);
        return QString("%1%2%3").arg(re).arg(sign).arg(imagPart);
    }
}

TComplex TComplex::fromString(const QString& str) {
    QString s = str.trimmed();
    s.replace(" ", "");

    if (s.isEmpty()) return TComplex(0, 0);

    // Проверка на простое вещественное число
    bool ok;
    double val = s.toDouble(&ok);
    if (ok && !s.contains('i')) {
        return TComplex(val, 0);
    }

    // Обработка комплексных чисел
    if (s == "i") return TComplex(0, 1);
    if (s == "-i") return TComplex(0, -1);

    // Убираем i в конце
    bool hasI = s.contains('i');
    if (hasI) {
        s = s.replace('i', "");
    }

    // Поиск знака мнимой части
    int plusPos = s.lastIndexOf('+');
    int minusPos = s.lastIndexOf('-');

    // Если знака нет - это либо вещественное, либо мнимое число
    if (plusPos <= 0 && minusPos <= 0) {
        if (hasI) {
            // Мнимое число
            return TComplex(0, s.toDouble());
        } else {
            // Вещественное число
            return TComplex(s.toDouble(), 0);
        }
    }

    // Определяем позицию разделителя
    int splitPos = (plusPos > minusPos) ? plusPos : minusPos;

    // Извлекаем части
    QString realPartStr = s.left(splitPos);
    QString imagPartStr = s.mid(splitPos);

    double realPart = realPartStr.isEmpty() ? 0 : realPartStr.toDouble();
    double imagPart = imagPartStr.isEmpty() ? 0 : imagPartStr.toDouble();

    if (hasI && imagPart == 0 && !imagPartStr.isEmpty()) {
        // Случай типа "1+i" или "1-i"
        imagPart = (imagPartStr == "+") ? 1 : -1;
    }

    return TComplex(realPart, imagPart);
}
