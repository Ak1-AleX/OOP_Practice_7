#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include "polynom.h"
#include "array.h"
#include <QVector>
#include <QString>

// Шаблон класса ФУНКЦИЯ согласно методичке
template<typename number>
class TFunction : protected Polynom<number> {
public:
    // Конструктор: степень, массив значений производных разложения Маклорена
    TFunction(int degree, const QVector<number>& derivatives);

    // Деструктор
    virtual ~TFunction();

    // Метод вычисления значения (наследуется от полинома)
    number value(const number& x) const;

    // Оператор вызова функции
    number operator()(const number& x) const;

    // Получение степени полинома
    int getDegree() const;

    // Получение строкового представления
    QString toString() const;

protected:
    // Вспомогательный метод для преобразования производных в коэффициенты
    void derivativesToCoefficients(int degree, const QVector<number>& derivatives);
};

#endif // FUNCTION_H
