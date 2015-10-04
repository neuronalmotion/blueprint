#ifndef SHAPEBEZIER_H
#define SHAPEBEZIER_H

#include <QList>
#include <QGraphicsPathItem>
#include <QImage>

#include "Shape.h"
#include "BoundingBox.h"
#include "BezierElement.h"

namespace blueprint {

class GraphicsItem;

class ShapeBezier : public Shape
{
public:
    explicit ShapeBezier(Shape* parentShape, const ShapeType& shapeType, const qreal& x, const qreal& y);
    ~ShapeBezier();

    QGraphicsItem* graphicsItem() override;
    void boundingBoxEvent(const BoundingBoxEvent& event);
    QRectF bounds() const override;
    void resizeOnCreation(const QPointF& delta) override;
    void setBackgroundColor(const QColor& color) override;
    QColor backgroundColor() const override;
    void setBorderColor(const QColor& color);
    int borderWidth() const override;
    void setBorderWidth(int width) override;

    void addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos);
    void addSegment(const QPointF& point);
    void closePath();
    void updateElement(BezierElement* bezierElement, const QPointF& pos);
    inline BoundingBox& boundingBox() { return mBoundingBox; }

    void setBackgroundImage(const QString& fileName);
    inline QImage* backgroundImage() { return mBackgroundImage; }
    inline QString backgroundImageFileName() { return mBackgroundImageFileName; }

protected:
    void updateBoundingBoxBezierVisibility();

protected:
    QPainterPath mPath;
    QList<BezierElement*> mElements;
    bool mIsPathClosed;
    GraphicsItem* mGraphicsItem;
    BoundingBox mBoundingBox;
    QImage* mBackgroundImage;
    QString mBackgroundImageFileName;
};

class GraphicsItem : public QGraphicsPathItem
{
public:
    GraphicsItem(ShapeBezier* shape, QGraphicsItem* parent = 0);
    ~GraphicsItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    ShapeBezier* mShape;
};

}

#endif // SHAPEBEZIER_H
