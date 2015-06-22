QT       += core widgets

CONFIG += c++11

TARGET = blueprint
TEMPLATE = app

INCLUDEPATH += "src"

SOURCES += src/main.cpp\
        src/MainWindow.cpp

HEADERS  += src/MainWindow.h

FORMS    += src/MainWindow.ui
