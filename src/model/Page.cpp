#include "Page.h"

using namespace blueprint;

Page::Page(Shape* parentShape)
    : ShapeRectangle(parentShape, ShapeType::PAGE, 0, 0)
{
    setBorderColor(QColor(10, 10, 10));
    setBackgroundColor(QColor(100, 100, 100));
    boundingBox().boundingBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, QPointF(10000.0, 10000.0));
}

Page::~Page()
{
}

SerializeInfo* Page::serialize() const
{
    SerializeInfo* serializeInfo = Shape::serialize();
    serializeInfo->setName("page");
    return serializeInfo;
}

