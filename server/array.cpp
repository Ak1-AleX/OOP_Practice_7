#include "array.h"
#include <cmath>
#include <algorithm>
#include <QDebug>
#include "complex.h"

using namespace std;

template<typename T>
Array<T>::Array() : data() {}

template<typename T>
Array<T>::Array(int n, T val) : data(n, val) {}

template<typename T>
Array<T>::Array(const Array& other) : data(other.data) {}

template<typename T>
void Array<T>::resize(int newSize) {
    if (newSize > 0) {
        data.resize(newSize);
    } else {
        data.clear();
    }
}

template<typename T>
void Array<T>::setElement(int index, T val) {
    if (index >= 0 && index < data.size()) {
        data[index] = val;
    }
}

template<typename T>
T Array<T>::getElement(int index) const {
    if (index >= 0 && index < data.size()) {
        return data[index];
    }
    return T(0.0);
}

template<typename T>
T Array<T>::average() const {
    if (data.isEmpty()) return T(0.0);

    double sumRe = 0, sumIm = 0;
    for (const T& num : data) {
        sumRe += num.real();
        sumIm += num.imag();
    }
    return T(sumRe / data.size(), sumIm / data.size());
}

template<typename T>
T Array<T>::deviation() const {
    if (data.isEmpty()) return T(0.0);

    T avg = average();
    double sum = 0;
    for (const T& num : data) {
        double dRe = num.real() - avg.real();
        double dIm = num.imag() - avg.imag();
        double distance = sqrt(dRe * dRe + dIm * dIm);
        sum += distance * distance;
    }
    return T(sqrt(sum / data.size() - 1), 0);
}

template<typename T>
void Array<T>::sortAsc() {
    if (!data.isEmpty()) {
        sort(data.begin(), data.end(), [](const T& a, const T& b) {
            return a.compare(b) < 0;
        });
    }
}

template<typename T>
void Array<T>::sortDesc() {
    if (!data.isEmpty()) {
        sort(data.begin(), data.end(), [](const T& a, const T& b) {
            return a.compare(b) > 0;
        });
    }
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this != &other) {
        data = other.data;
    }
    return *this;
}

// Явная инстанциация шаблонов
template class Array<TComplex>;
