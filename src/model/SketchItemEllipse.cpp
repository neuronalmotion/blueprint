#include "SketchItemEllipse.h"

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
    // because the path is closed, we don't have to
    // move the first item, the last is enough
    switch (direction) {

        default:
            qErrnoWarning("Unexpected TranslationDirection!");
            break;
    }
    mBoundingBox->updateRect(direction);
}

