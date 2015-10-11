#include "Canvas.h"

#include "Shape.h"

using namespace blueprint;

Canvas::Canvas(Shape* parentShape, const qreal& x, const qreal& y)
    : ShapeRectangle(parentShape, ShapeType::CANVAS, x, y)
{

    //setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setBorderColor(QColor(10, 10, 10));
    setBackgroundColor(QColor(255, 255, 255));
    boundingBox().boundingBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, QPointF(300.0, 500.0));
}

Canvas::~Canvas()
{
}

Parcel* Canvas::toParcel() const
{
    Parcel* parcel = Shape::toParcel();
    parcel->setName("canvas");
    return parcel;
}
