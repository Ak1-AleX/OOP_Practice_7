#pragma once
#ifndef SIFUNCTION_H
#define SIFUNCTION_H

#include "sinfunction.h"
#include <QVector>

// Шаблон класса для функции Si(x) = sin(x)/x
template<typename number>
class TFSi : public TFsin<number> {
public:
    TFSi(int degree);
    virtual ~TFSi();

    // Переопределяем value для Si(x)
    number value(const number& x) const;

    // Оператор вызова функции
    number operator()(const number& x) const;

private:
    // Метод для вычисления производных Si(x) в точке 0
    QVector<number> calculateDerivativesSi(int degree) const;

    // Указатель на массив производных
    number* d;
};

#endif // SIFUNCTION_H
