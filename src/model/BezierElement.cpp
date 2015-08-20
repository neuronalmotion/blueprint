#include "BezierElement.h"

#include <QGraphicsItem>

#include "Shape.h"

BezierElement::BezierElement(ElementType elementType, Shape* parent, const int index)
    : mElementType(elementType),
    mParent(parent),
    mIndex(index)
{

}

BezierElement::~BezierElement()
{

}

void BezierElement::propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
         mParent->updateElement(this, value.toPointF());
    }
}
