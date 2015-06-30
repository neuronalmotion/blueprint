#ifndef BEZIERCONTROLPOINT_H
#define BEZIERCONTROLPOINT_H

#include <QGraphicsRectItem>

class SketchItemBezier;

class BezierControlPoint : public QGraphicsRectItem
{
public:
    BezierControlPoint(SketchItemBezier* item, const int index);
    ~BezierControlPoint();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    SketchItemBezier* mItem;
    int mIndex;
};

#endif // BEZIERCONTROLPOINT_H
