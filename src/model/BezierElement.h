#ifndef BEZIERELEMENT_H
#define BEZIERELEMENT_H

#include <QVariant>
#include <QGraphicsItem>

class BezierPath;

class BezierElement
{
public:

    enum ElementType {
        POINT,
        CONTROL_POINT
    };

    BezierElement(ElementType elementType, BezierPath* parent, const int index);
    virtual ~BezierElement();

    virtual QPointF getPos() = 0;

    inline ElementType getElementType() const { return mElementType; }
    inline int getIndex() const { return mIndex; }

protected:
    void propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

protected:
    ElementType mElementType;
    BezierPath* mParent;
    int mIndex;
};

#endif // BEZIERELEMENT_H
