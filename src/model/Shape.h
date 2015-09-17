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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    void addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos);
    void closePath();
    void updateElement(BezierElement* bezierElement, const QPointF& pos);
    void boundingBoxEvent(const BoundingBoxEvent& event);
    virtual void collapse() = 0;
    virtual void resizeOnCreation(const QPointF& delta);

    inline ShapeType shapeType() const { return mShapeType; }
    void setSelected(bool selected) override;
    inline EditMode editMode() const { return mEditMode; }
    void toggleEditMode();
    virtual void setEditMode(const EditMode& mode);
    QRectF bounds() const;
    inline BoundingBox& boundingBox() { return mBoundingBox; }
    QPointF posAbsolute();

    void setBackgroundColor(const QColor& color);
    inline QColor backgroundColor() { return brush().color(); }
    void setBorderColor(const QColor& color);
    void setBackgroundImage(const QString& fileName);
    inline QImage* backgroundImage() { return mBackgroundImage; }
    inline QString backgroundImageFileName() { return mBackgroundImageFileName; }

protected:
    virtual void updateBoundingBoxBezierVisibility();

    QPainterPath mPath;
    ShapeType mShapeType;
    QList<BezierElement*> mElements;
    BoundingBox mBoundingBox;
    bool mIsPathClosed;
    EditMode mEditMode;
    QImage* mBackgroundImage;
    QString mBackgroundImageFileName;
};

}

#endif // SHAPE_H
