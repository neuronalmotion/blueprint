#include "SketchItemRectangle.h"

#include <QDebug>

// Constants
static const uint TOP_LEFT_INDEX        = 12;
static const uint TOP_RIGHT_INDEX       = 3;
static const uint BOTTOM_RIGHT_INDEX    = 6;
static const uint BOTTOM_LEFT_INDEX     = 9;

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
    // because the path is closed, we don't have to
    // move the first item, the last is enough
    switch (direction) {

        case BoundingBoxPoint::TOP_LEFT:
            mElements[TOP_LEFT_INDEX]->moveBy(delta);
            mElements[TOP_RIGHT_INDEX]->moveBy(QPointF(0, delta.y()));
            mElements[BOTTOM_LEFT_INDEX]->moveBy(QPointF(delta.x(), 0));
            break;

        case BoundingBoxPoint::TOP:
            mElements[TOP_RIGHT_INDEX]->moveBy(delta);
            mElements[TOP_LEFT_INDEX]->moveBy(delta);
            break;

        case BoundingBoxPoint::TOP_RIGHT:
            mElements[TOP_RIGHT_INDEX]->moveBy(delta);
            mElements[TOP_LEFT_INDEX]->moveBy(QPointF(0, delta.y()));
            mElements[BOTTOM_RIGHT_INDEX]->moveBy(QPointF(delta.x(), 0));
            break;

        case BoundingBoxPoint::RIGHT:
            mElements[TOP_RIGHT_INDEX]->moveBy(delta);
            mElements[TOP_LEFT_INDEX]->moveBy(delta);
            break;

        default:
            qErrnoWarning("Unexpected TranslationDirection!");
            break;
    }
    mBoundingBox->updateRect(direction);
}

