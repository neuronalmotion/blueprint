#ifndef BEZIERCONTROLPOINT_H
#define BEZIERCONTROLPOINT_H

#include <QGraphicsRectItem>

#include "BezierElement.h"

namespace blueprint {
class Shape;

class BezierControlPoint : public BezierElement, public QGraphicsRectItem
{
public:
    BezierControlPoint(Shape* parent, const int index);
    ~BezierControlPoint();

    QPointF pos() const override;
    void setPos(QPointF pos) override;
    void moveBy(QPointF delta) override;
    void setVisible(bool isVisible) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
}

#endif // BEZIERCONTROLPOINT_H
