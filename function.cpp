#include "function.h"
#include "complex.h"
#include <cmath>
#include <QDebug>

template<typename number>
TFunction<number>::TFunction(int degree, const QVector<number>& derivatives)
    : Polynom<number>(degree, number(1.0, 0.0)) {

    if (derivatives.size() < degree + 1) {
        qDebug() << "Ошибка: недостаточно производных для степени" << degree;
        return;
    }

    derivativesToCoefficients(degree, derivatives);
}

template<typename number>
TFunction<number>::~TFunction() {
    // Деструктор по умолчанию
}

template<typename number>
number TFunction<number>::value(const number& x) const {
    return this->evaluate(x);
}

template<typename number>
number TFunction<number>::operator()(const number& x) const {
    return value(x);
}

template<typename number>
int TFunction<number>::getDegree() const {
    return Polynom<number>::getDegree();
}

template<typename number>
QString TFunction<number>::toString() const {
    return Polynom<number>::toStringForm1();
}

template<typename number>
void TFunction<number>::derivativesToCoefficients(int degree, const QVector<number>& derivatives) {
    // Преобразуем производные в коэффициенты ряда Маклорена
    // f(x) ≈ f(0) + f'(0)x + f''(0)x²/2! + f'''(0)x³/3! + ...

    for (int n = 0; n <= degree; n++) {
        number coeff = derivatives[n];

        // Делим на n!
        double factorial = 1.0;
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }

        // Делим коэффициент на факториал
        coeff = coeff / number(factorial, 0.0);

        // Записываем коэффициент (доступ к protected полю)
        this->coefficients.setElement(n, coeff);
    }
}

// Явная инстанциация для типа number (TComplex)
template class TFunction<TComplex>;
