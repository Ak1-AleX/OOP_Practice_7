QT += core gui widgets

CONFIG += c++11

SOURCES += \
    array.cpp \
    complex.cpp \
    function.cpp \
    main.cpp \
    mainwindow.cpp \
    polynom.cpp \
    sinfunction.cpp \
    sifunction.cpp

HEADERS += \
    array.h \
    complex.h \
    function.h \
    mainwindow.h \
    number.h \
    polynom.h \
    sinfunction.h \
    sifunction.h

FORMS += \
    mainwindow.ui

TEMPLATE = app
TARGET = pr7

# Настройки компилятора
QMAKE_CXXFLAGS += -std=c++11
