#ifndef SKETCHITEM_H
#define SKETCHITEM_H

#include <QGraphicsItem>
#include <QRectF>

#include "model/GraphicalItem.h"
#include "model/BoundingBox.h"
#include "model/BoundingBoxPoint.h"

class SketchItem : public GraphicalItem
{
public:

    enum Type {
        SKETCH_ITEM_BEZIER,
        BOUNDING_BOX_POINT,
        BEZIER_POINT,
        BEZIER_CONTROL_POINT
    };

    enum EditMode {
        BOUNDING_BOX,
        BEZIER
    };

    SketchItem(GraphicalItem* parentItem = 0);
    virtual ~SketchItem();

    virtual QGraphicsItem* getGraphicsItem() = 0;
    virtual void setIsSelected(bool isSelected) { }
    virtual void setEditMode(EditMode mode);
    virtual void boundingBoxEvent(const BoundingBoxEvent& event) = 0;
    virtual QRectF getBounds() { getGraphicsItem()->boundingRect(); }

protected:
    EditMode mEditMode;


};

#endif // SKETCHITEM_H
