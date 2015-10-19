#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <QString>
#include <QPointF>

#include "Shape.h"

namespace blueprint {

class Page;
class Canvas;
class ShapeBezier;

class ShapeFactory
{
public:
    static Shape* createRectangle(Shape* shapeParent = nullptr, bool addBezierPoints = true);
    static Page* createPage(bool addBezierPoints = true);
    static Canvas* createCanvas(Shape* shapeParent = nullptr, bool addBezierPoints = true);
    static Shape* createEllipse(Shape* shapeParent = nullptr, bool addBezierPoints = true);

    static Shape* createShape(const Shape::ShapeType shapeType,
                              Shape* shapeParent = nullptr,
                              bool addBezierPoints = true);

private:
    ShapeFactory();

    static void addRectanglePoints(ShapeBezier& shape);
    static void updateGraphicsItemParenting(Shape* parent, Shape* child);

};
}

#endif // SHAPEFACTORY_H
