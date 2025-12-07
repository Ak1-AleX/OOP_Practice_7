#include "sinfunction.h"
#include "complex.h"
#include <cmath>
#include <QDebug>

template<typename number>
TFsin<number>::TFsin(int degree)
    : TFunction<number>(degree, calculateDerivatives(degree)) {
}

template<typename number>
TFsin<number>::~TFsin() {
    // Деструктор по умолчанию
}

template<typename number>
QVector<number> TFsin<number>::calculateDerivatives(int degree) const {
    QVector<number> derivatives;
    derivatives.reserve(degree + 1);

    // Производные sin(x) в точке 0: 0, 1, 0, -1, 0, 1, 0, -1, ...
    for (int n = 0; n <= degree; n++) {
        switch (n % 4) {
        case 0: // sin(0) = 0
            derivatives.append(number(0.0, 0.0));
            break;
        case 1: // cos(0) = 1
            derivatives.append(number(1.0, 0.0));
            break;
        case 2: // -sin(0) = 0
            derivatives.append(number(0.0, 0.0));
            break;
        case 3: // -cos(0) = -1
            derivatives.append(number(-1.0, 0.0));
            break;
        }
    }

    return derivatives;
}

// Явная инстанциация для типа number (TComplex)
template class TFsin<TComplex>;
