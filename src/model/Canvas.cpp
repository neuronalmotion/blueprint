#include "Canvas.h"

#include "Shape.h"

using namespace blueprint;

Canvas::Canvas(Shape* parentShape)
    : ShapeBezier(parentShape, ShapeType::CANVAS)
{

    //setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setBorderColor(QColor(10, 10, 10));
    setBackgroundColor(QColor(255, 255, 255));
}

Canvas::~Canvas()
{
}

Parcel* Canvas::toParcel() const
{
    Parcel* parcel = ShapeBezier::toParcel();
    parcel->setName("canvas");
    return parcel;
}
