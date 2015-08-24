#ifndef BEZIERELEMENT_H
#define BEZIERELEMENT_H

#include <QGraphicsItem>

namespace blueprint {
class Shape;

class BezierElement
{
public:

    enum ElementType {
        POINT,
        CONTROL_POINT
    };

    BezierElement(ElementType elementType, Shape* parent, int index);
    virtual ~BezierElement();

    virtual QPointF pos() const = 0;
    virtual void setPos(const QPointF& pos) = 0;
    virtual void moveBy(const QPointF& delta) = 0;
    virtual void setVisible(bool isVisible) = 0;

    inline ElementType elementType() const { return mElementType; }
    inline int index() const { return mIndex; }

protected:
    void propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

protected:
    ElementType mElementType;
    Shape* mParentShape;
    const int mIndex;
};

}

#endif // BEZIERELEMENT_H
