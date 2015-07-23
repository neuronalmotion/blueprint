#include "SketchItemRectangle.h"

#include <QDebug>

SketchItemRectangle::SketchItemRectangle(qreal x, qreal y)
    : SketchItemBezier(x, y)
{
    addPath(QPointF(40, 0), QPointF(60, 0), QPointF(100, 0));
    addPath(QPointF(100, 20), QPointF(100, 40), QPointF(100, 50));
    addPath(QPointF(60, 50), QPointF(40, 50), QPointF(0, 50));
    addPath(QPointF(0, 40), QPointF(0, 20), QPointF(0, 0));
    closePath();
    mBoundingBox->updateRect();
}

void SketchItemRectangle::boundBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta)
{
    QList<BezierElement*> elementsToMove;
    // because the path is closed, we don't have to
    // move the first item, the last is enough
    switch (direction) {
    case BoundingBoxPoint::TOP_LEFT:
        elementsToMove.append(mElements.last()); // top left
        mElements[3]->moveBy(QPointF(0, delta.y())); // top right
        mElements[9]->moveBy(QPointF(delta.x(), 0)); // bottom left
        break;
    case BoundingBoxPoint::TOP:
        elementsToMove.append(mElements[3]); // top right
        elementsToMove.append(mElements.last()); // top left
        break;

    case BoundingBoxPoint::TOP_RIGHT:
        elementsToMove.append(mElements[3]); // top right
        mElements.last()->moveBy(QPointF(0, delta.y())); // top left
        mElements[6]->moveBy(QPointF(delta.x(), 0)); // bottom right
        break;
    default:
        break;
    }

    for (auto e : elementsToMove) {
        e->moveBy(delta);
    }
    mBoundingBox->updateRect();
}

