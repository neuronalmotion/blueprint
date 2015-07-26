#include "SketchItemEllipse.h"

#include <QDebug>
#include <QGraphicsEllipseItem>


// Constants
static const uint TOP_INDEX     = 12;
static const uint RIGHT_INDEX   = 3;
static const uint BOTTOM_INDEX  = 6;
static const uint LEFT_INDEX    = 9;

SketchItemEllipse::SketchItemEllipse(qreal x, qreal y)
    : SketchItemBezier(x, y)
{
    addPath(QPointF(30, 0), QPointF(50, 20), QPointF(50, 50));
    addPath(QPointF(50, 80), QPointF(30, 100), QPointF(0, 100));
    addPath(QPointF(-40, 100), QPointF(-50, 80), QPointF(-50, 50));
    addPath(QPointF(-40, 20), QPointF(-30, 0), QPointF(0, 0));
    closePath();
    mBoundingBox->updateRect();
}

SketchItemEllipse::~SketchItemEllipse()
{

}

void SketchItemEllipse::boundBoxPointMoved(BoundingBoxPoint::TranslationDirection direction, QPointF delta)
{
    QPointF halfDelta = QPointF(delta.x() / 2.0f, delta.y() / 2.0f);

    // because the path is closed, we don't have to
    // move the first item, the last is enough
    switch (direction) {

        case BoundingBoxPoint::TOP:
            mElements[TOP_INDEX]->moveBy(QPointF(0.0f, delta.y()));
            mElements[RIGHT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            mElements[LEFT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            break;

        case BoundingBoxPoint::TOP_RIGHT:
            mElements[TOP_INDEX]->moveBy(QPointF(0.0f, delta.y()));
            mElements[RIGHT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            mElements[LEFT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));

            mElements[RIGHT_INDEX]->moveBy(QPointF(delta.x(), 0.0f));
            mElements[TOP_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            mElements[BOTTOM_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            break;

        case BoundingBoxPoint::RIGHT:
            mElements[RIGHT_INDEX]->moveBy(QPointF(delta.x(), 0.0f));
            mElements[TOP_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            mElements[BOTTOM_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            break;

        case BoundingBoxPoint::BOTTOM_RIGHT:
            mElements[BOTTOM_INDEX]->moveBy(QPointF(0.0f, delta.y()));
            mElements[RIGHT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            mElements[LEFT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));

            mElements[RIGHT_INDEX]->moveBy(QPointF(delta.x(), 0.0f));
            mElements[TOP_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            mElements[BOTTOM_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            break;

        case BoundingBoxPoint::BOTTOM:
            mElements[BOTTOM_INDEX]->moveBy(QPointF(0.0f, delta.y()));
            mElements[RIGHT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            mElements[LEFT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            break;

        case BoundingBoxPoint::BOTTOM_LEFT:
            mElements[BOTTOM_INDEX]->moveBy(QPointF(0.0f, delta.y()));
            mElements[RIGHT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            mElements[LEFT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));

            mElements[LEFT_INDEX]->moveBy(QPointF(delta.x(), 0.0f));
            mElements[TOP_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            mElements[BOTTOM_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            break;

        case BoundingBoxPoint::LEFT:
            mElements[LEFT_INDEX]->moveBy(QPointF(delta.x(), 0.0f));
            mElements[TOP_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            mElements[BOTTOM_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            break;

        case BoundingBoxPoint::TOP_LEFT:
            mElements[TOP_INDEX]->moveBy(QPointF(0.0f, delta.y()));
            mElements[RIGHT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));
            mElements[LEFT_INDEX]->moveBy(QPointF(0.0f, halfDelta.y()));

            mElements[LEFT_INDEX]->moveBy(QPointF(delta.x(), 0.0f));
            mElements[TOP_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            mElements[BOTTOM_INDEX]->moveBy(QPointF(halfDelta.x(), 0.0f));
            break;

        default:
            qErrnoWarning("Unexpected TranslationDirection!");
            break;
    }
    mBoundingBox->updateRect(direction);
}

