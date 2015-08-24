#include "BezierElement.h"

#include <QGraphicsItem>

#include "Shape.h"

using namespace blueprint;

BezierElement::BezierElement(ElementType elementType, Shape* parent, int index)
    : mElementType(elementType),
    mParentShape(parent),
    mIndex(index)
{

}

BezierElement::~BezierElement()
{

}

void BezierElement::propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
         mParentShape->updateElement(this, value.toPointF());
    }
}
