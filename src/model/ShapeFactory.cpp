#include "ShapeFactory.h"

#include "Canvas.h"
#include "Page.h"
#include "Shape.h"
#include "ShapeBezier.h"
#include "ShapeLine.h"
#include "ShapeText.h"
#include "ShapeBezierCurve.h"

using namespace blueprint;

ShapeFactory::ShapeFactory()
{

}

void ShapeFactory::addRectanglePoints(ShapeBezier& shape)
{
    shape.addPath(QPointF(40, 0), QPointF(60, 0), QPointF(100, 0));
    shape.addPath(QPointF(100, 20), QPointF(100, 40), QPointF(100, 50));
    shape.addPath(QPointF(60, 50), QPointF(40, 50), QPointF(0, 50));
    shape.addPath(QPointF(0, 40), QPointF(0, 20), QPointF(0, 0));
}

void ShapeFactory::updateGraphicsItemParenting(Shape* parent, Shape* child)
{
    if (parent && child) {
        child->graphicsItem()->setParentItem(parent->graphicsItem());
    }
}

Shape* ShapeFactory::createRectangle(Shape* shapeParent, bool addBezierPoints)
{
    ShapeBezier* shape = new ShapeBezier(shapeParent);
    shape->setName("Rectangle");
    if (addBezierPoints) {
        addRectanglePoints(*shape);
        shape->closePath();
        shape->boundingBox()->updateRect();
    }
    updateGraphicsItemParenting(shapeParent, shape);
    return shape;
}

Page* ShapeFactory::createPage(bool addBezierPoints)
{
    Page* shape = new Page();
    shape->setName("Page");
    if (addBezierPoints) {
        addRectanglePoints(*shape);
        shape->closePath();
        shape->boundingBox()->updateRect();
        shape->boundingBox()->boundingBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, QPointF(10000.0, 10000.0));
    }
    return shape;
}

Canvas* ShapeFactory::createCanvas(Shape* shapeParent, bool addBezierPoints)
{
    Canvas* shape = new Canvas(shapeParent);
    shape->setName("Canvas");
    if (addBezierPoints) {
        addRectanglePoints(*shape);
        shape->closePath();
        shape->boundingBox()->updateRect();
        shape->boundingBox()->boundingBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, QPointF(300.0, 500.0));
    }
    updateGraphicsItemParenting(shapeParent, shape);
    return shape;
}

Shape* ShapeFactory::createEllipse(Shape* shapeParent, bool addBezierPoints)
{
    ShapeBezier* shape = new ShapeBezier(shapeParent);
    shape->setName("Ellipse");
    if (addBezierPoints) {
        shape->addPath(QPointF(30, 0), QPointF(50, 20), QPointF(50, 50));
        shape->addPath(QPointF(50, 80), QPointF(30, 100), QPointF(0, 100));
        shape->addPath(QPointF(-40, 100), QPointF(-50, 80), QPointF(-50, 50));
        shape->addPath(QPointF(-50, 20), QPointF(-30, 0), QPointF(0, 0));
        shape->closePath();
        shape->boundingBox()->updateRect();
    }
    updateGraphicsItemParenting(shapeParent, shape);
    return shape;
}

static uint id = 0;
Shape* ShapeFactory::createShape(const Shape::ShapeType shapeType, Shape* shapeParent, bool addBezierPoints)
{
    Shape* shape = nullptr;
    QString name;
    switch (shapeType) {
    case Shape::ShapeType::PAGE:
        shape = ShapeFactory::createPage(addBezierPoints);
        name = "Page";
        break;

    case Shape::ShapeType::CANVAS:
        shape = ShapeFactory::createCanvas(shapeParent, addBezierPoints);
        name = "Canvas";
        break;

    case Shape::ShapeType::RECTANGLE:
        shape = ShapeFactory::createRectangle(shapeParent, addBezierPoints);
        name = "Rectangle";
        break;

    case Shape::ShapeType::ELLIPSE:
        shape = ShapeFactory::createEllipse(shapeParent, addBezierPoints);
        name = "Ellipse";
        break;

    case Shape::ShapeType::LINE:
        shape = new ShapeLine(shapeParent);
        name = "Line";
        break;
    case Shape::ShapeType::TEXT:
        shape = new ShapeText(shapeParent);
        name = "Text";
        break;
    case Shape::ShapeType::BEZIER_CURVE:
        shape = new ShapeBezierCurve(shapeParent);
        name = "Bezier curve";
        break;
    default:
        shape = new ShapeBezier(shapeParent);
        break;
    }
    Q_ASSERT(shape != nullptr);
    shape->setName(QString("%1 %2").arg(name).arg(id++));
    if (shapeParent) {
        shape->graphicsItem()->setParentItem(shapeParent->graphicsItem());
    }
    return shape;
}
