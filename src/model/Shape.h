#ifndef SHAPE_H
#define SHAPE_H

#include <QColor>
#include <QBrush>
#include <QPainter>

#include "TreeItem.h"
#include "BoundingBox.h"

class QPointF;

namespace blueprint {
class BezierPath;

class Shape : public TreeItem, public QGraphicsPathItem
{
public:

    enum ShapeType {
        CANVAS,
        ELLIPSE,
        LINE,
        RECTANGLE,
    };

    enum EditMode {
        BOUNDING_BOX,
        BEZIER
    };
    static Shape* fromQGraphicsItem(const QGraphicsItem& item);


    explicit Shape(TreeItem* parentTreeItem, const ShapeType shapeType, qreal x, qreal y);
    virtual ~Shape();

    virtual QGraphicsItem* getQGraphicsItem() = 0;
    virtual void collapse() = 0;
    virtual QRectF bounds() const = 0;
    virtual void resizeOnCreation(const QPointF& delta) = 0;
    virtual void boundingBoxEvent(const BoundingBoxEvent& event) = 0;
    virtual void setBackgroundColor(const QColor& color) = 0;
    virtual QColor backgroundColor() const = 0;
    virtual void setBorderColor(const QColor& color) = 0;

    inline ShapeType shapeType() const { return mShapeType; }
    void setSelected(bool selected) override;
    inline EditMode editMode() const { return mEditMode; }
    void toggleEditMode();
    virtual void setEditMode(const EditMode& mode);
    inline BoundingBox& boundingBox() { return mBoundingBox; }
    QPointF posAbsolute();


protected:
    virtual void updateBoundingBoxBezierVisibility() = 0;

    ShapeType mShapeType;
    BoundingBox mBoundingBox;
    EditMode mEditMode;
};
}

#endif // SHAPE_H
