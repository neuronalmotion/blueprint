#ifndef BEZIERCONTROLPOINT_H
#define BEZIERCONTROLPOINT_H

#include <QGraphicsRectItem>

#include "BezierElement.h"

namespace blueprint {
class ShapeBezier;

class BezierControlPoint : public BezierElement, public QGraphicsRectItem
{
public:
    BezierControlPoint(ShapeBezier* parent, const int index = 0);
    ~BezierControlPoint();

    QPointF pos() const override;
    void setPos(const QPointF& pos) override;
    void moveBy(const QPointF& delta) override;
    void setVisible(bool isVisible) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
}

#endif // BEZIERCONTROLPOINT_H
