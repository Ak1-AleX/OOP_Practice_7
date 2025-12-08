#pragma once
#ifndef POLYNOM_H
#define POLYNOM_H

#include "array.h"
#include <QString>
#include <QVector>

template<typename T>
class Polynom {
protected:
    Array<T> coefficients;

private:
    Array<T> roots;
    int degree;
    T leadingCoeff;

    void calculateCoefficients();

public:
    Polynom();
    Polynom(int n, T an = T(1.0));
    Polynom(int n); // Конструктор для полинома x^n

    T evaluate(T x) const;

    int getDegree() const { return degree; }
    T getCoefficient(int index) const;
    T getLeadingCoeff() const { return leadingCoeff; }
    void setLeadingCoeff(T an);
    void setRoot(int index, T root);
    T getRoot(int index) const;

    bool isEmpty() const { return degree == 0; }

    QString toStringForm1() const;
    QString toStringForm2() const;
    void setFromRoots(int n, T an, const QVector<T>& rootsList);
};

#endif // POLYNOM_H
