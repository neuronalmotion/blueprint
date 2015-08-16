#include "Canvas.h"

#include "Shape.h"

Canvas::Canvas(GraphicalItem* parentItem, qreal x, qreal y)
    : ShapeRectangle(parentItem, x, y)
{

    //mItem->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    mItem->setData(Shape::Type::CANVAS, qVariantFromValue(static_cast<void *>(this)));
}

Canvas::~Canvas()
{

}
