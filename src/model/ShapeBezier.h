#ifndef SHAPEBEZIER_H
#define SHAPEBEZIER_H

#include <QList>
#include <QGraphicsPathItem>
#include <QImage>

#include "Shape.h"
#include "BezierElement.h"

namespace blueprint {

class GraphicsItem;

class ShapeBezier : public Shape
{
public:
    explicit ShapeBezier(Shape* parentShape, const ShapeType& shapeType, const qreal& x = 0, const qreal& y = 0);
    explicit ShapeBezier(Shape* parentShape);
    ~ShapeBezier();

    QGraphicsItem* graphicsItem() const override;
    void boundingBoxEvent(const BoundingBoxEvent& event) override;
    QRectF bounds() const override;
    void resizeOnCreation(const QPointF& delta) override;
    void setForegroundColor(const QColor& color) override;
    QColor foregroundColor() const override;
    void setBorderColor(const QColor& color);
    int borderWidth() const override;
    void setBorderWidth(int width) override;

    void addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos);
    void addSegment(const QPointF& point);
    void closePath();
    inline int elementCount() const { return mElements.length(); }
    inline const BezierElement* element(const int& index) const { return mElements[index]; }
    void updateElement(BezierElement* bezierElement, const QPointF& pos);

    void setBackgroundImage(const QString& fileNamei);
    inline QImage* backgroundImage() { return mBackgroundImage; }
    inline QString backgroundImageFileName() { return mBackgroundImageFileName; }

    Parcel* toParcel() const override;
    void fromParcel(const Parcel& parcel) override;
    void bezierElementsFromParcel(const Parcel& parcel);

protected:
    void updateBoundingBoxBezierVisibility();

private:
    void init(qreal x, qreal y);

protected:
    QPainterPath mPath;
    QList<BezierElement*> mElements;
    bool mIsPathClosed;
    GraphicsItem* mGraphicsItem;
    QImage* mBackgroundImage;
    QString mBackgroundImageFileName;
};

class GraphicsItem : public QGraphicsPathItem
{
public:
    GraphicsItem(ShapeBezier* shape, QGraphicsItem* parent = 0);
    ~GraphicsItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    ShapeBezier* mShape;
};

}

#endif // SHAPEBEZIER_H
