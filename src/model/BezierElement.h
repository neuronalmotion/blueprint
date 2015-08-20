#ifndef BEZIERELEMENT_H
#define BEZIERELEMENT_H

#include <QVariant>
#include <QGraphicsItem>

class Shape;

class BezierElement
{
public:

    enum ElementType {
        POINT,
        CONTROL_POINT
    };

    BezierElement(ElementType elementType, Shape* parent, const int index);
    virtual ~BezierElement();

    virtual QPointF getPos() = 0;
    virtual void setPos(QPointF pos) = 0;
    virtual void moveBy(QPointF delta) = 0;
    virtual void setVisible(bool isVisible) = 0;

    inline ElementType getElementType() const { return mElementType; }
    inline int getIndex() const { return mIndex; }
    inline Shape* getParentSketchItemBezier(){ return mParent; }

protected:
    void propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

protected:
    ElementType mElementType;
    Shape* mParent;
    int mIndex;
};

#endif // BEZIERELEMENT_H
