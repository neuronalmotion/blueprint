#include "SketchItemBezier.h"

#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

#include "BezierPath.h"
#include "BezierElement.h"

SketchItemBezier::SketchItemBezier()
    : SketchItem(),
      mItem(new QGraphicsPathItem),
      mPath(),
      mPathes()
{
}

SketchItemBezier::~SketchItemBezier()
{
    delete mItem;
    for (auto p : mPathes) {
        delete p;
        p = nullptr;
    }
    mPathes.clear();
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
    int startElementIndex = mPath.elementCount() <= 0 ? 0 : mPath.elementCount() - 1;
    mPath.cubicTo(c1, c2, endPos);
    for (int i = 0; i < mPath.elementCount(); ++i) {
         QPainterPath::Element element = mPath.elementAt(i);
         qDebug() << "Element:" << element << element.type;
    }
    BezierPath* p = new BezierPath(this, &mPath, startElementIndex);
    if (!mPathes.empty()) {
        BezierPath* const lastPath = mPathes.last();
        lastPath->setNextPath(p);
    }
    mPathes.append(p);
    mItem->setPath(mPath);
}

void SketchItemBezier::updateElement(BezierElement* bezierElement, const QPointF& pos)
{
    mPath.setElementPositionAt(bezierElement->getIndex(), pos.x(), pos.y());
    mItem->setPath(mPath);
}
