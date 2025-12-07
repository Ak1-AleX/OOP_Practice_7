QT += core network

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = PolynomServer
TEMPLATE = app

SOURCES += \
    main.cpp \
    serverApp.cpp \
    polynom.cpp \
    array.cpp \
    complex.cpp \
    ../common/message.cpp \
    ../common/communicator.cpp

HEADERS += \
    serverApp.h \
    polynom.h \
    array.h \
    complex.h \
    number.h \
    ../common/message.h \
    ../common/communicator.h

# Настройки для сборки
OBJECTS_DIR = .obj
MOC_DIR = .moc

# Пути для включения заголовков
INCLUDEPATH += ../common
DEPENDPATH += ../common
