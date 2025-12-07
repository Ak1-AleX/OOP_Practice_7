#include "polynom.h"
#include <cmath>
#include <QString>
#include <QVector>
#include <QDebug>
#include "complex.h"

template<typename T>
Polynom<T>::Polynom() : degree(0), leadingCoeff(T(1.0, 0.0)) {
    coefficients = Array<T>(1, T(0.0, 0.0));
    roots = Array<T>(0);
}

template<typename T>
Polynom<T>::Polynom(int n, T an) : degree(n), leadingCoeff(an) {
    if (n > 0) {
        coefficients = Array<T>(n + 1, T(0.0, 0.0));
        roots = Array<T>(n, T(0.0, 0.0));
        coefficients.setElement(n, an);
    } else {
        degree = 0;
        coefficients = Array<T>(1, T(0.0, 0.0));
        roots = Array<T>(0);
    }
}

template<typename T>
void Polynom<T>::calculateCoefficients() {
    if (degree <= 0) {
        coefficients = Array<T>(1, leadingCoeff);
        return;
    }

    coefficients = Array<T>(degree + 1, T(0.0, 0.0));
    coefficients.setElement(degree, leadingCoeff);

    for (int k = 1; k <= degree; k++) {
        T sum = T(0.0, 0.0);

        for (int i = 0; i < degree; i++) {
            T product = T(1.0, 0.0);
            int count = 0;

            for (int j = i; j < degree && count < k; j++) {
                product = product * roots.getElement(j);
                count++;
            }

            if (count == k) {
                sum = sum + product;
            }
        }

        if (k % 2 == 0) {
            coefficients.setElement(degree - k, leadingCoeff * sum);
        } else {
            coefficients.setElement(degree - k, T(-1.0, 0.0) * leadingCoeff * sum);
        }
    }
}

template<typename T>
T Polynom<T>::evaluate(T x) const {
    if (isEmpty()) return T(0.0, 0.0);

    T result = coefficients.getElement(degree);
    for (int i = degree - 1; i >= 0; i--) {
        result = result * x + coefficients.getElement(i);
    }
    return result;
}

template<typename T>
T Polynom<T>::getCoefficient(int index) const {
    if (index >= 0 && index <= degree) {
        return coefficients.getElement(index);
    }
    return T(0.0, 0.0);
}

template<typename T>
void Polynom<T>::setLeadingCoeff(T an) {
    leadingCoeff = an;
    calculateCoefficients();
}

template<typename T>
void Polynom<T>::setRoot(int index, T root) {
    if (index >= 0 && index < degree) {
        roots.setElement(index, root);
        calculateCoefficients();
    }
}

template<typename T>
T Polynom<T>::getRoot(int index) const {
    if (index >= 0 && index < degree) {
        return roots.getElement(index);
    }
    return T(0.0, 0.0);
}

template<typename T>
QString Polynom<T>::toStringForm1() const {
    if (isEmpty()) return "P(x) = 0";

    QString result = "P(x) = ";
    bool firstTerm = true;

    for (int i = degree; i >= 0; i--) {
        T coeff = coefficients.getElement(i);

        if (coeff == T(0.0, 0.0)) continue;

        if (!firstTerm) {
            if (coeff.real() >= 0 && coeff.imag() >= 0) {
                result += " + ";
            } else {
                result += " - ";
                coeff = T(-coeff.real(), -coeff.imag());
            }
        }

        result += "(" + coeff.toString() + ")";

        if (i > 0) {
            result += "x";
            if (i > 1) {
                result += QString("^%1").arg(i);
            }
        }

        firstTerm = false;
    }

    if (firstTerm) {
        result += "0";
    }
    return result;
}

template<typename T>
QString Polynom<T>::toStringForm2() const {
    if (isEmpty()) return "P(x) = 0";

    QString result = "P(x) = " + leadingCoeff.toString();
    for (int i = 0; i < degree; i++) {
        T root = roots.getElement(i);
        result += QString("(x - (%1))").arg(root.toString());
    }
    return result;
}

template<typename T>
void Polynom<T>::setFromRoots(int n, T an, const QVector<T>& rootsList) {
    degree = n;
    leadingCoeff = an;

    if (n > 0) {
        roots = Array<T>(n, T(0.0, 0.0));
        for (int i = 0; i < n && i < rootsList.size(); i++) {
            roots.setElement(i, rootsList[i]);
        }
        calculateCoefficients();
    } else {
        coefficients = Array<T>(1, T(0.0, 0.0));
        roots = Array<T>(0);
    }
}

// Явная инстанциация шаблонов
template class Polynom<TComplex>;
