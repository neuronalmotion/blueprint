QT       += core widgets

CONFIG += c++11

TARGET = blueprint
TEMPLATE = app

INCLUDEPATH += "src"

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/CanvasView.cpp \
    src/Tool.cpp \
    src/model/SketchItem.cpp \
    src/model/SketchItemBezier.cpp \
    src/model/Page.cpp \
    src/model/Sketch.cpp

HEADERS  += src/MainWindow.h \
    src/CanvasView.h \
    src/Tool.h \
    src/model/SketchItem.h \
    src/model/SketchItemBezier.h \
    src/model/Page.h \
    src/model/Sketch.h

FORMS    += src/MainWindow.ui
