#include "SketchItemBezier.h"

#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

#include "BezierControlPoint.h"

SketchItemBezier::SketchItemBezier()
    : SketchItem(),
      mItem(new QGraphicsPathItem),
      mPath(),
      mStartPoint(new QGraphicsEllipseItem(mItem)),
      mEndPoint(new QGraphicsEllipseItem(mItem)),
      mControl1(new BezierControlPoint(this, 0)),
      mControl2(new BezierControlPoint(this, 1))
{
//    int circleSize = 10;
//    QPointF start = QPointF(50, 50);
//    QPointF end = QPointF(300, 100);
//    mItem->setPos(start);

//    mStartPoint->setRect(-circleSize/2, -circleSize/2, circleSize, circleSize);
//    mEndPoint->setRect(-circleSize/2, -circleSize/2, circleSize, circleSize);
//    QPen pen = QColor::fromRgb(255, 0, 0);
//    mStartPoint->setPen(pen);

//    mEndPoint->setPen(pen);
//    mEndPoint->setPos(end);

//    mControl1->setPos(100, 0);
//    mControl2->setPos(end - QPointF(0, 100));

//    QPointF c1 = mControl1->pos();
//    QPointF cscene1 = mControl1->scenePos();

//    QPainterPath path(QPointF(0, 0));
//    path.cubicTo(mControl1->pos(), mControl2->pos(), mEndPoint->pos());
//    mItem->setPath(path);
}

SketchItemBezier::~SketchItemBezier()
{
    delete mItem;
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
    int currentElement = mPath.elementCount() - 1;
    mPath.cubicTo(c1, c2, endPos);
    for (int i = 0; i < mPath.elementCount(); ++i) {
         QPainterPath::Element element = mPath.elementAt(i);
         qDebug() << element << element.type;
    }
    mItem->setPath(mPath);
}

void SketchItemBezier::updateElement(int index, const QPointF& pos)
{
    mPath.setElementPositionAt(index, pos.x(), pos.y());
    mItem->setPath(mPath);
}

