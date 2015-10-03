QT       += core widgets

CONFIG += c++11

TARGET = blueprint
TEMPLATE = app

INCLUDEPATH += "src"

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/CanvasView.cpp \
    src/Tool.cpp \
    src/model/BezierControlPoint.cpp \
    src/model/BezierPoint.cpp \
    src/model/Page.cpp \
    src/model/Blueprint.cpp \
    src/model/BezierElement.cpp \
    src/model/BoundingBox.cpp \
    src/model/BoundingBoxPoint.cpp \
    src/model/Canvas.cpp \
    src/model/ShapeEllipse.cpp \
    src/model/ShapeRectangle.cpp \
    src/model/Shape.cpp \
    src/PropertiesWindow.cpp \
    src/model/ShapeLine.cpp \
    src/model/ShapeBezier.cpp \
    src/model/ShapeModel.cpp \
    src/TreeView.cpp

HEADERS  += src/MainWindow.h \
    src/CanvasView.h \
    src/Tool.h \
    src/model/BezierControlPoint.h \
    src/model/BezierPoint.h \
    src/model/Page.h \
    src/model/Blueprint.h \
    src/model/BezierElement.h \
    src/model/BoundingBox.h \
    src/model/BoundingBoxPoint.h \
    src/model/Canvas.h \
    src/model/ShapeEllipse.h \
    src/model/ShapeRectangle.h \
    src/model/Shape.h \
    src/PropertiesWindow.h \
    src/model/ShapeLine.h \
    src/model/ShapeBezier.h \
    src/model/ShapeModel.h \
    src/TreeView.h

FORMS    += src/MainWindow.ui \
    src/PropertiesWindow.ui
