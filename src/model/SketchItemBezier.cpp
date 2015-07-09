#include "SketchItemBezier.h"

#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

#include "BezierElement.h"
#include "BezierControlPoint.h"
#include "BezierPoint.h"

SketchItemBezier::SketchItemBezier()
    : SketchItem(),
      mItem(new QGraphicsPathItem),
      mPath(),
      mElements()
{
}

SketchItemBezier::~SketchItemBezier()
{
    delete mItem;
    for (auto p : mElements) {
        delete p;
        p = nullptr;
    }
    mElements.clear();
}

QGraphicsItem* SketchItemBezier::getGraphicsItem()
{
    return mItem;
}

void SketchItemBezier::addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos)
{
    qDebug() << "Adding path: " << mPath.currentPosition()
             << "=>" << endPos
             << "[c1" << c1
             << "c2" << c2 << "]";

    int startElementIndex = mPath.elementCount() <= 0 ? 0 : mPath.elementCount();
    mPath.cubicTo(c1, c2, endPos);

    // create bezier elements
    int elementCount = mPath.elementCount();
    for (int i = startElementIndex; i < elementCount; ++i) {
         QPainterPath::Element element = mPath.elementAt(i);
         qDebug() << "Element:" << element << element.type;

         BezierElement* bezierElement = nullptr;
         // start point only if we are starting a path
         if ((startElementIndex == 0 && i == 0)
                 || i == elementCount - 1) {
             bezierElement = new BezierPoint(this, i);
         } else {
             bezierElement = new BezierControlPoint(this, i);
         }
         bezierElement->setPos(QPointF(element.x, element.y));
         mElements.append(bezierElement);
    }

    mItem->setPath(mPath);
}

void SketchItemBezier::updateElement(BezierElement* bezierElement, const QPointF& pos)
{
    int listIndex = mElements.indexOf(bezierElement);

    if(listIndex >= 0 && bezierElement->getElementType() == BezierElement::POINT) {
        QPointF delta = pos - bezierElement->getPos();

        if (bezierElement == mElements[0]) {
            mElements[listIndex + 1]->moveBy(delta);

        } else if (bezierElement == mElements.last()) {
            mElements[listIndex - 1]->moveBy(delta);

        } else {
            mElements[listIndex - 1]->moveBy(delta);
            mElements[listIndex + 1]->moveBy(delta);
        }
    }
    mPath.setElementPositionAt(bezierElement->getIndex(), pos.x(), pos.y());
    mItem->setPath(mPath);
}
