#include "Canvas.h"

#include "Shape.h"

using namespace blueprint;

Canvas::Canvas(TreeItem* parentItem, qreal x, qreal y)
    : ShapeRectangle(parentItem, x, y)
{

    mItemType = TreeItem::ItemType::CANVAS;
    //setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setBorderColor(QColor(10, 10, 10));
    setBackgroundColor(QColor(255, 255, 255));
    boundingBox().boundingBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, QPointF(300.0, 500.0));
}

Canvas::~Canvas()
{
}
