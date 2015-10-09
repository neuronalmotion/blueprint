#include "ShapeFactory.h"

#include "Canvas.h"
#include "Page.h"
#include "Shape.h"
#include "ShapeRectangle.h"
#include "ShapeEllipse.h"
#include "ShapeLine.h"
#include "ShapeText.h"

using namespace blueprint;

ShapeFactory::ShapeFactory()
{

}

Shape* ShapeFactory::createShape(const Shape::ShapeType shapeType, Shape& shapeParent)
{
    return ShapeFactory::createShape(shapeType, shapeParent, QPointF(0, 0));
}

static uint id = 0;
Shape* ShapeFactory::createShape(const Shape::ShapeType shapeType,
                                 Shape& shapeParent,
                                 const QPointF& position)
{

    Shape* shape = nullptr;
    QString name;
    switch (shapeType) {
    case Shape::ShapeType::PAGE:
        shape = new Page();
        name = "Page";
        break;

    case Shape::ShapeType::CANVAS:
        shape = new Canvas(&shapeParent, position.x(), position.y());
        name = "Canvas";
        break;

    case Shape::ShapeType::RECTANGLE:
        shape = new ShapeRectangle(&shapeParent, position.x(), position.y());
        name = "Rectangle";
        break;

    case Shape::ShapeType::ELLIPSE:
        shape = new ShapeEllipse(&shapeParent, position.x(), position.y());
        name = "Ellipse";
        break;

    case Shape::ShapeType::LINE:
        shape = new ShapeLine(&shapeParent, position.x(), position.y());
        name = "Line";
        break;
    case Shape::ShapeType::TEXT:
        shape = new ShapeText(&shapeParent, position.x(), position.y());
        name = "Text";
        break;
    default:
        break;
    }
    Q_ASSERT(shape != nullptr);
    shape->setName(QString("%1 %2").arg(name).arg(id++));
    shape->collapse();
    shape->graphicsItem()->setParentItem(shapeParent.graphicsItem());
    return shape;
}
