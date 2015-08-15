#include "Shape.h"

#include <QDebug>

Shape::Shape(GraphicalItem* parentItem)
    : GraphicalItem(parentItem),
    mEditMode(EditMode::BOUNDING_BOX)
{

}

Shape::~Shape()
{

}

void Shape::setEditMode(EditMode mode)
{
    qDebug() << "SketchItem::setEditMode()";
    mEditMode = mode;
}
