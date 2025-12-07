#pragma once
#ifndef SINFUNCTION_H
#define SINFUNCTION_H

#include "function.h"
#include <QVector>

// Шаблон класса для функции sin(x)
template<typename number>
class TFsin : public TFunction<number> {
public:
    TFsin(int degree);
    virtual ~TFsin();

    // Метод для вычисления значения
    number value(const number& x) const {
        return TFunction<number>::value(x);
    }

private:
    // Метод для вычисления производных синуса в точке 0
    QVector<number> calculateDerivatives(int degree) const;
};

#endif // SINFUNCTION_H
