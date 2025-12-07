QT += widgets core network

CONFIG += c++17

TARGET = PolynomClient
TEMPLATE = app

macx {
    CONFIG += app_bundle
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    clientApp.cpp \
    ../common/message.cpp \
    ../common/communicator.cpp

HEADERS += \
    mainwindow.h \
    clientApp.h \
    ../common/message.h \
    ../common/communicator.h

FORMS += \
    mainwindow.ui

# Настройки для сборки
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui

# Пути для включения заголовков
INCLUDEPATH += ../common
DEPENDPATH += ../common
