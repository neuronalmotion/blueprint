#include "SketchItemBezier.h"

#include <QPainterPath>
#include <QPen>

SketchItemBezier::SketchItemBezier()
    : SketchItem(),
      mItem(new QGraphicsPathItem),
      mStartPoint(new QGraphicsEllipseItem(mItem)),
      mEndPoint(new QGraphicsEllipseItem(mItem)),
      mControl1(new QGraphicsRectItem(mItem)),
      mControl2(new QGraphicsRectItem(mItem))
{
    int circleSize = 10;
    QPointF start = QPointF(50, 50);
    QPointF end = QPointF(300, 100);
    mItem->setPos(start);

    mStartPoint->setRect(-circleSize/2, -circleSize/2, circleSize, circleSize);
    mEndPoint->setRect(-circleSize/2, -circleSize/2, circleSize, circleSize);
    QPen pen = QColor::fromRgb(255, 0, 0);
    mStartPoint->setPen(pen);

    mEndPoint->setPen(pen);
    mEndPoint->setPos(end);

    int rectSize = 10;
    mControl1->setRect(-rectSize/2, -rectSize/2, rectSize, rectSize);
    mControl2->setRect(-rectSize/2, -rectSize/2, rectSize, rectSize);
    mControl1->setPos(100, 0);
    mControl2->setPos(end - QPointF(0, 100));

    QPointF c1 = mControl1->pos();
    QPointF cscene1 = mControl1->scenePos();

    QPainterPath path(QPointF(0, 0));
    path.cubicTo(mControl1->pos(), mControl2->pos(), mEndPoint->pos());
    mItem->setPath(path);
}

SketchItemBezier::~SketchItemBezier()
{
    delete mItem;
}

QGraphicsItem* SketchItemBezier::getGraphicsItem()
{
    return mItem;
}

