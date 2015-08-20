#ifndef SHAPEBEZIER_H
#define SHAPEBEZIER_H

#include "Shape.h"

#include <QList>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>

#include "BezierElement.h"
#include "BoundingBox.h"

class QPointF;
class BezierPath;

class ShapeBezier : public Shape
{
public:
    ShapeBezier(GraphicalItem* parentItem, qreal x, qreal y);
    ~ShapeBezier();
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
};

#endif // SHAPEBEZIER_H
