#ifndef SHAPE_H
#define SHAPE_H

#include <QList>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>

#include "TreeItem.h"
#include "BezierElement.h"
#include "BoundingBox.h"

class QPointF;
class BezierPath;

class Shape : public TreeItem
{
public:

    // FIXME Rename to ShapeType
    enum Type {
        CANVAS,
        SHAPE_BEZIER,
        BOUNDING_BOX_POINT,
        BEZIER_POINT,
        BEZIER_CONTROL_POINT
    };

    enum EditMode {
        BOUNDING_BOX,
        BEZIER
    };

    Shape(TreeItem* parentItem, qreal x, qreal y);
    ~Shape();
    virtual QGraphicsItem* getGraphicsItem();
    void addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos);
    void closePath();
    void updateElement(BezierElement* bezierElement, const QPointF& pos);
    void boundingBoxEvent(const BoundingBoxEvent& event);

    void setSelected(bool selected) override;
    void setEditMode(const EditMode& mode);
    QRectF getBounds();
    void setBackgroundColor(const QColor& color);
    void setBorderColor(const QColor& color);
    inline BoundingBox* boundingBox(){ return mBoundingBox; }

protected:
    void updateBoundingBoxBezierVisibility();

    QGraphicsPathItem* mItem;
    QPainterPath mPath;
    QList<BezierElement*> mElements;
    BoundingBox* mBoundingBox;
    bool mIsPathClosed;
    EditMode mEditMode;
};

#endif // SHAPE_H
