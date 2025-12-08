#include "sifunction.h"
#include "complex.h"
#include <cmath>
#include <QDebug>

template<typename number>
TFSi<number>::TFSi(int degree)
    : TFsin<number>(degree), d(nullptr) {

    // Вычисляем производные для Si(x)
    QVector<number> derivatives = calculateDerivativesSi(degree);

    // Сохраняем производные
    d = new number[degree + 1];
    for (int i = 0; i <= degree; i++) {
        d[i] = derivatives[i];
    }

    // Преобразуем производные в коэффициенты
    for (int n = 0; n <= degree; n++) {
        number coeff = derivatives[n];

        // Делим на n!
        double factorial = 1.0;
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }

        coeff = coeff / number(factorial, 0.0);

        // Записываем коэффициент
        this->coefficients.setElement(n, coeff);
    }
}

template<typename number>
TFSi<number>::~TFSi() {
    if (d) {
        delete [] d;
    }
}

template<typename number>
number TFSi<number>::value(const number& x) const {
    // Для x = 0 используем предел: Si(0) = 1
    if (x.real() == 0.0 && x.imag() == 0.0) {
        return number(1.0, 0.0);
    }

    // Для остальных точек: Si(x) = sin(x)/x
    return TFsin<number>::value(x) / x;
}

template<typename number>
number TFSi<number>::operator()(const number& x) const {
    return value(x);
}

template<typename number>
QVector<number> TFSi<number>::calculateDerivativesSi(int degree) const {
    QVector<number> derivatives;
    derivatives.reserve(degree + 1);

    // Производные Si(x) = sin(x)/x в точке 0
    for (int n = 0; n <= degree; n++) {
        if (n == 0) {
            derivatives.append(number(1.0, 0.0));  // Si(0) = 1
        } else if (n % 2 == 1) {
            derivatives.append(number(0.0, 0.0));  // Нечетные производные = 0
        } else {
            // Четные производные: Si^(2k)(0) = (-1)^k / (2k+1)
            int k = n / 2;
            double value = (k % 2 == 0) ? 1.0 : -1.0;
            value /= (2 * k + 1);
            derivatives.append(number(value, 0.0));
        }
    }

    return derivatives;
}

// Явная инстанциация для типа number (TComplex)
template class TFSi<TComplex>;
