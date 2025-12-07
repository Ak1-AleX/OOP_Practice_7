#pragma once

#include <QVector>

template<typename T>
class Array {
private:
    QVector<T> data;

public:
    Array();
    Array(int n, T val = T(0.0));
    Array(const Array& other);
    ~Array() = default;

    void resize(int newSize);
    void setElement(int index, T val);
    T getElement(int index) const;

    T average() const;
    T deviation() const;

    void sortAsc();
    void sortDesc();

    bool isEmpty() const { return data.isEmpty(); }
    int getSize() const { return data.size(); }

    Array& operator=(const Array& other);
};
