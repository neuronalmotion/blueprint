#include "Page.h"

using namespace blueprint;

Page::Page(Shape* parentShape)
    : ShapeBezier(parentShape, ShapeType::PAGE, 0, 0)
{
    setBorderColor(QColor(10, 10, 10));
    setBackgroundColor(QColor(100, 100, 100));
}

Page::~Page()
{
}

Parcel* Page::toParcel() const
{
    Parcel* parcel = ShapeBezier::toParcel();
    parcel->setName("page");
    return parcel;
}

