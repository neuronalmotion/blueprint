#ifndef BEZIERELEMENT_H
#define BEZIERELEMENT_H

#include <QVariant>
#include <QGraphicsItem>

class SketchItemBezier;

class BezierElement
{

public:
    BezierElement(SketchItemBezier* item, const int index);
    virtual ~BezierElement();

protected:
    void propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

protected:
    SketchItemBezier* mSketchItem;
    int mIndex;
};

#endif // BEZIERELEMENT_H
