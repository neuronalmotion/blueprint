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
    explicit ShapeText(Shape* parentShape);
    ~ShapeText();

    QGraphicsItem* graphicsItem() override;
    QRectF bounds() const override;
    void resizeOnCreation(const QPointF& delta) override;
    void boundingBoxEvent(const BoundingBoxEvent& event) override;
    void setBackgroundColor(const QColor& color) override;
    QColor backgroundColor() const override;
    void setBorderColor(const QColor& color) override;
    int borderWidth() const override;
    void setBorderWidth(int width) override;
    inline virtual BoundingBox& boundingBox() override { return mBoundingBox; }

    QString text() const;
    void setText(const QString& text);

    QFont font() const;
    void setFont(const QFont& font);

protected:
    void updateBoundingBoxBezierVisibility() override;

private:
    QGraphicsTextItem* mGraphicsItem;
    BoundingBox mBoundingBox;
};
}

#endif // SHAPETEXT_H
