QT       += core widgets

CONFIG += c++11

TARGET = blueprint
TEMPLATE = app

INCLUDEPATH += "src"

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/CanvasView.cpp \
    src/Tool.cpp

HEADERS  += src/MainWindow.h \
    src/CanvasView.h \
    src/Tool.h

FORMS    += src/MainWindow.ui
