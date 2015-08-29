#ifndef SHAPE_H
#define SHAPE_H

#include <QList>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>
#include <QColor>
#include <QBrush>
#include <QPainter>

#include "TreeItem.h"
#include "BezierElement.h"
#include "BoundingBox.h"

class QPointF;

namespace blueprint {
class BezierPath;

class Shape : public TreeItem, public QGraphicsPathItem
{
public:

    enum ShapeType {
        CANVAS,
        SHAPE,
        BOUNDING_BOX_POINT,
        BEZIER_POINT,
        BEZIER_CONTROL_POINT
    };

    enum EditMode {
        BOUNDING_BOX,
        BEZIER
    };

    Shape(TreeItem* parentTreeItem, qreal x, qreal y);
    virtual ~Shape();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    void addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos);
    void closePath();
    void updateElement(BezierElement* bezierElement, const QPointF& pos);
    void boundingBoxEvent(const BoundingBoxEvent& event);

    void setSelected(bool selected) override;
    inline EditMode editMode() const { return mEditMode; }
    void toggleEditMode();
    void setEditMode(const EditMode& mode);
    QRectF bounds() const;
    void setBackgroundColor(const QColor& color);
    void setBorderColor(const QColor& color);
    inline BoundingBox& boundingBox() { return mBoundingBox; }
    inline QColor backgroundColor() { return brush().color(); }

    QPointF posAbsolute();

protected:
    void updateBoundingBoxBezierVisibility();

    QPainterPath mPath;
    QList<BezierElement*> mElements;
    BoundingBox mBoundingBox;
    bool mIsPathClosed;
    EditMode mEditMode;
};

}

#endif // SHAPE_H
