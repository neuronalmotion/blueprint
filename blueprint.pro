QT       += core widgets

CONFIG += c++11

TARGET = blueprint
TEMPLATE = app

INCLUDEPATH += "src"

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
    src/TreeView.h \
    src/model/ShapeFactory.h \
    src/model/ShapeText.h \
    src/io/XmlInputOutput.h \
    src/io/Parcelable.h \
    src/io/Parcel.h

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
    src/TreeView.cpp \
    src/model/ShapeFactory.cpp \
    src/model/ShapeText.cpp \
    src/io/XmlInputOutput.cpp \
    src/io/Parcel.cpp

FORMS    += src/MainWindow.ui \
    src/PropertiesWindow.ui

############################################
# Test section
############################################

test {
    message(Test build)

    QT += testlib
    TARGET = blueprint-test
    SOURCES -= src/main.cpp

    INCLUDEPATH += "test"

    HEADERS += \
        test/TestUtils.h \
        test/TestExample.h \
        test/TestSerialization.h

    SOURCES += \
        test/main_test.cpp \
        test/TestUtils.cpp \
        test/TestExample.cpp \
        test/TestSerialization.cpp

} else {
    message(Normal build)
}
