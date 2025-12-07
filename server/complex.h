#pragma once
#include <cmath>
#include <QString>

class TComplex {
private:
    double re;
    double im;

public:
    TComplex();
    TComplex(double r, double i = 0.0);

    double real() const { return re; }
    double imag() const { return im; }

    void setReal(double r) { re = r; }
    void setImag(double i) { im = i; }

    TComplex operator+(const TComplex& other) const;
    TComplex operator-(const TComplex& other) const;
    TComplex operator*(const TComplex& other) const;
    TComplex operator/(const TComplex& other) const;

    TComplex& operator=(const TComplex& other) {
        if (this != &other) {
            re = other.re;
            im = other.im;
        }
        return *this;
    }

    bool operator==(const TComplex& other) const;
    bool operator!=(const TComplex& other) const;

    double abs() const;
    int compare(const TComplex& other) const;

    QString toString() const;
    static TComplex fromString(const QString& str);
};
