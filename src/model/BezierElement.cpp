#include "BezierElement.h"

#include <QGraphicsItem>

#include "model/SketchItemBezier.h"

BezierElement::BezierElement(SketchItemBezier* item, const int index)
      : mSketchItem(item),
      mIndex(index)
{

}

BezierElement::~BezierElement()
{

}

void BezierElement::propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
         mSketchItem->updateElement(mIndex, value.toPointF());
    }
}
