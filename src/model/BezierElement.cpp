#include "BezierElement.h"

#include <QGraphicsItem>

#include "ShapeBezier.h"
#include "BezierPoint.h"
#include "BezierControlPoint.h"

using namespace blueprint;

BezierElement* BezierElement::bezierElementFromParcel(const Parcel& parcel, ShapeBezier* parent)
{
    ElementType type = static_cast<ElementType>(parcel.propertyValue("type").toInt());
    BezierElement* element;
    switch (type) {
    case POINT:
        element = new BezierPoint(parent);
    break;

    case CONTROL_POINT:
        element = new BezierControlPoint(parent);
    break;

    default:
        qFatal("Invalid element type");
    break;
    }
    element->fromParcel(parcel);
    return element;
}

BezierElement::BezierElement(ElementType elementType, ShapeBezier* parent, int index)
    : mElementType(elementType),
    mParentShape(parent),
    mIndex(index),
    mPropagateItemChange(true)
{

}

BezierElement::~BezierElement()
{

}

Parcel* BezierElement::toParcel() const
{
    Parcel* parcel = new Parcel("element");
    QPointF pos = this->pos();
    parcel->putProperty("posx", pos.x());
    parcel->putProperty("posy", pos.y());
    parcel->putProperty("type", mElementType);
    parcel->putProperty("index", mIndex);
    return parcel;
}

void BezierElement::fromParcel(const Parcel& parcel)
{
    QPointF pos(parcel.propertyValue("posx").toFloat(),
                parcel.propertyValue("posy").toFloat());
    mPropagateItemChange = false;
    setPos(pos);
    mElementType = static_cast<ElementType>(parcel.propertyValue("type").toInt());
    mIndex = parcel.propertyValue("index").toInt();
    mPropagateItemChange = true;
}

void BezierElement::propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (!mPropagateItemChange) {
        return;
    }
    if (change == QGraphicsItem::ItemPositionChange) {
         mParentShape->updateElement(this, value.toPointF());
    }
}
