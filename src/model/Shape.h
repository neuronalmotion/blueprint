#ifndef SHAPE_H
#define SHAPE_H

#include <QColor>
#include <QPersistentModelIndex>

#include "BoundingBox.h"

class QPointF;

namespace blueprint {
class BezierPath;

class Shape
{
public:

    enum ShapeType {
        PAGE,
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

    explicit Shape(Shape* parentShape, const ShapeType& shapeType);
    virtual ~Shape();

    // hierarchy stuff
    Shape* child(int row);
    void appendChild(Shape* child);
    int indexOf(const Shape* child) const;
    int childCount() const;
    int columnCount() const;
    int row() const;

    // drawing stuff
    virtual QGraphicsItem* graphicsItem() = 0;
    virtual void collapse() = 0;
    virtual QRectF bounds() const = 0;
    virtual void resizeOnCreation(const QPointF& delta) = 0;
    virtual void boundingBoxEvent(const BoundingBoxEvent& event) = 0;
    virtual void setBackgroundColor(const QColor& color) = 0;
    virtual QColor backgroundColor() const = 0;
    virtual void setBorderColor(const QColor& color) = 0;

    inline ShapeType shapeType() const { return mShapeType; }
    void setSelected(bool selected);
    inline EditMode editMode() const { return mEditMode; }
    void toggleEditMode();
    virtual void setEditMode(const EditMode& mode);
    QPointF posAbsolute();

    inline QString name() const { return mName; }
    inline void setName(const QString& name) { mName = name; }
    inline Shape* parentShape() const {return mParentShape; }
    void setParentShape(Shape* parentShape);
    inline QPersistentModelIndex* modelIndex() const { return mModelIndex; }
    inline void setModelIndex(const QModelIndex& index) { mModelIndex = new QPersistentModelIndex(index); }


protected:
    virtual void updateBoundingBoxBezierVisibility() = 0;

    Shape* mParentShape;
    ShapeType mShapeType;
    QString mName;
    EditMode mEditMode;
    QList<Shape*> mChildItems;
    QPersistentModelIndex* mModelIndex;
    bool mIsSelected;
};
}

#endif // SHAPE_H
