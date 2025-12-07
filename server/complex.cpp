#include "complex.h"
#include <cmath>
#include <QString>

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
    else if (re == 0) return QString::number(im) + "i";
    else return QString("%1%2%3i").arg(re).arg(im >= 0 ? "+" : "").arg(im);
}

TComplex TComplex::fromString(const QString& str) {
    QString s = str.trimmed();
    if (s.contains('i')) {
        if (s.contains('+') || (s.contains('-') && s.lastIndexOf('-') > 0)) {
            int opPos = qMax(s.lastIndexOf('+'), s.lastIndexOf('-'));
            double realPart = s.left(opPos).toDouble();
            double imagPart = s.mid(opPos, s.length() - opPos - 1).toDouble();
            return TComplex(realPart, imagPart);
        } else {
            double imagPart = s.remove('i').toDouble();
            return TComplex(0, imagPart);
        }
    } else {
        return TComplex(s.toDouble(), 0);
    }
}
