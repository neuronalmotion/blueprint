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
    src/model/SketchItemEllipse.cpp \
    src/model/BezierControlPoint.cpp \
    src/model/BezierPoint.cpp \
    src/model/Page.cpp \
    src/model/Sketch.cpp \
    src/model/Blueprint.cpp \
    src/model/BezierElement.cpp \
    src/model/SketchItemRectangle.cpp \
    src/model/BoundingBox.cpp \
    src/model/BoundingBoxPoint.cpp

HEADERS  += src/MainWindow.h \
    src/CanvasView.h \
    src/Tool.h \
    src/model/SketchItem.h \
    src/model/SketchItemBezier.h \
    src/model/SketchItemEllipse.h \
    src/model/BezierControlPoint.h \
    src/model/BezierPoint.h \
    src/model/Page.h \
    src/model/Sketch.h \
    src/model/Blueprint.h \
    src/model/BezierElement.h \
    src/model/SketchItemRectangle.h \
    src/model/BoundingBox.h \
    src/model/BoundingBoxPoint.h

FORMS    += src/MainWindow.ui
