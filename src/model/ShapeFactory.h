#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <QString>
#include <QPointF>

#include "Shape.h"

namespace blueprint {

class ShapeFactory
{
public:
    static Shape* createShape(const Shape::ShapeType shapeType,
                              Shape& shapeParent,
                              const QPointF& position);
private:
    ShapeFactory();

};
}

#endif // SHAPEFACTORY_H
