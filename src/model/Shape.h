#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItem>
#include <QRectF>

#include "model/GraphicalItem.h"
#include "model/BoundingBox.h"
#include "model/BoundingBoxPoint.h"

class Shape : public GraphicalItem
{
public:

    enum Type {
        SHAPE_BEZIER,
        BOUNDING_BOX_POINT,
        BEZIER_POINT,
        BEZIER_CONTROL_POINT
    };

    enum EditMode {
        BOUNDING_BOX,
        BEZIER
    };

    Shape(GraphicalItem* parentItem);
    virtual ~Shape();

    virtual void setIsSelected(bool isSelected) { }
    virtual void setEditMode(EditMode mode);
    virtual void boundingBoxEvent(const BoundingBoxEvent& event) = 0;
    virtual QRectF getBounds() { getGraphicsItem()->boundingRect(); }
    virtual void setBackgroundColor(QColor color) { }
    virtual void setBorderColor(QColor color) { }

protected:
    EditMode mEditMode;


};

#endif // SHAPE_H
