#ifndef SHAPETEXT_H
#define SHAPETEXT_H

#include "Shape.h"

#include <QString>
#include <QFont>

#include "BoundingBox.h"

class QGraphicsTextItem;

namespace blueprint {

class ShapeText : public Shape
{
public:
    explicit ShapeText(Shape* parentShape, const ShapeType& shapeType, const qreal& x = 0, const qreal& y = 0);
    explicit ShapeText(Shape* parentShape);
    ~ShapeText();

    QGraphicsItem* graphicsItem() const override;
    QRectF bounds() const override;
    void resizeOnCreation(const QPointF& delta) override;
    void boundingBoxEvent(const BoundingBoxEvent& event) override;
    void setForegroundColor(const QColor& color) override;
    QColor foregroundColor() const override;
    void setBorderColor(const QColor& color) override;
    int borderWidth() const override;
    void setBorderWidth(int width) override;
    inline virtual BoundingBox& boundingBox() override { return mBoundingBox; }

    QString text() const;
    void setText(const QString& text);

    QFont font() const;
    void setFont(const QFont& font);

    Parcel* toParcel() const override;
    void fromParcel(const Parcel& parcel) override;

protected:
    void updateBoundingBoxBezierVisibility() override;

private:
    void init(qreal x, qreal y);

    QGraphicsTextItem* mGraphicsItem;
    BoundingBox mBoundingBox;
};

class TextGraphicsItem : public QGraphicsTextItem
{
public:
    TextGraphicsItem(ShapeText* shape, QGraphicsItem* parent = 0);
    ~TextGraphicsItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    ShapeText* mShape;

};

}

#endif // SHAPETEXT_H
