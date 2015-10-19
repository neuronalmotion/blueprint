#ifndef BEZIERELEMENT_H
#define BEZIERELEMENT_H

#include <QGraphicsItem>

#include "io/Parcelable.h"

namespace blueprint {
class ShapeBezier;

class BezierElement : public Parcelable
{
public:

    enum ElementType {
        POINT,
        CONTROL_POINT
    };

    static BezierElement* bezierElementFromParcel(const Parcel& parcel, ShapeBezier* parent);

    BezierElement(ElementType elementType, ShapeBezier* parent, int index);
    virtual ~BezierElement();

    virtual QPointF pos() const = 0;
    virtual void setPos(const QPointF& pos) = 0;
    virtual void moveBy(const QPointF& delta) = 0;
    virtual void setVisible(bool isVisible) = 0;

    inline ElementType elementType() const { return mElementType; }
    inline int index() const { return mIndex; }

    Parcel* toParcel() const override;
    void fromParcel(const Parcel& parcel) override;

protected:
    void propagateItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

protected:
    ElementType mElementType;
    ShapeBezier* mParentShape;
    int mIndex;

private:
    bool mPropagateItemChange;
};

}

#endif // BEZIERELEMENT_H
