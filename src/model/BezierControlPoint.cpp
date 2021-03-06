#include "BezierControlPoint.h"

#include <QPen>

#include "ShapeBezier.h"

using namespace blueprint;

BezierControlPoint::BezierControlPoint(ShapeBezier* parent, int index)
    : BezierElement(ElementType::CONTROL_POINT, parent, index),
    QGraphicsRectItem(parent->graphicsItem())
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    QPen pen = QPen(Qt::green);
    setPen(pen);

    int rectSize = 10;
    setRect(-rectSize/2, -rectSize/2, rectSize, rectSize);
    setVisible(false);
    setData(0, qVariantFromValue(static_cast<void *>(parent)));
}

BezierControlPoint::~BezierControlPoint()
{

}

QPointF BezierControlPoint::pos() const
{
    return QGraphicsItem::pos();
}

void BezierControlPoint::setPos(const QPointF& pos)
{
    QGraphicsItem::setPos(pos);
}

void BezierControlPoint::moveBy(const QPointF& delta)
{
    QGraphicsItem::moveBy(delta.x(), delta.y());
}

void BezierControlPoint::setVisible(bool isVisible)
{
    QGraphicsItem::setVisible(isVisible);
}

QVariant BezierControlPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    propagateItemChange(change, value);
    return QGraphicsItem::itemChange(change, value);
}


