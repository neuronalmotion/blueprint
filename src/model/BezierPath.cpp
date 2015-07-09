#include "BezierPath.h"

#include "SketchItemBezier.h"
#include <QPainterPath>

BezierPath::BezierPath(SketchItemBezier* item, const QPainterPath* path, const int elementStartIndex)
    : mItem(item),
    mElementStartIndex(elementStartIndex),
    mStartPoint(nullptr),
    mControl1(nullptr),
    mControl2(nullptr),
    mEndPoint(nullptr),
    mNextPath(nullptr)
{
    QPainterPath::Element element;

    if (elementStartIndex <= 0) {
        mStartPoint = new BezierPoint(this, elementStartIndex);
        element = path->elementAt(elementStartIndex);
        mStartPoint->setPos(element.x, element.y);
    }

    element = path->elementAt(elementStartIndex + 1);
    mControl1 = new BezierControlPoint(this, elementStartIndex + 1);
    mControl1->setPos(element.x, element.y);

    element = path->elementAt(elementStartIndex + 2);
    mControl2 = new BezierControlPoint(this, elementStartIndex + 2);
    mControl2->setPos(element.x, element.y);

    element = path->elementAt(elementStartIndex + 3);
    mEndPoint = new BezierPoint(this, elementStartIndex + 3);
    mEndPoint->setPos(element.x, element.y);
}

BezierPath::~BezierPath()
{
    if (mStartPoint){
        delete mStartPoint;
        mStartPoint = nullptr;
    }

    if (mControl1){
        delete mControl1;
        mControl1 = nullptr;
    }

    if (mControl2){
        delete mControl2;
        mControl2 = nullptr;
    }

    if (mEndPoint){
        delete mEndPoint;
        mEndPoint = nullptr;
    }

}

void BezierPath::updateElement(BezierElement* bezierElement, const QPointF& pos)
{
    if(bezierElement->getElementType() == BezierElement::POINT) {
        QPointF delta = pos - bezierElement->getPos();

        if (bezierElement == mStartPoint) {
            mControl1->moveBy(delta.x(), delta.y());

        } else if (bezierElement == mEndPoint) {
            mControl2->moveBy(delta.x(), delta.y());

            if (hasNextPath()) {
                mNextPath->mControl1->moveBy(delta.x(), delta.y());
            }
        }
    }
    mItem->updateElement(bezierElement, pos);
}

