#ifndef BOUNDINGBOXPOINT_H
#define BOUNDINGBOXPOINT_H

#include <QGraphicsRectItem>

class BoundingBox;

class BoundingBoxPoint : public QGraphicsRectItem
{
public:

    enum TranslationDirection {
        NONE,
        TOP_LEFT,
        TOP,
        TOP_RIGHT,
        RIGHT,
        BOTTOM_RIGHT,
        BOTTOM,
        BOTTOM_LEFT,
        LEFT,
    };

    BoundingBoxPoint(BoundingBox* parent, TranslationDirection direction);
    ~BoundingBoxPoint();

    TranslationDirection getTranslationDirection() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QPointF restrictPosition(const QPointF& newPosition);

private:
    BoundingBox* mParentBoundingBox;
    TranslationDirection mTranslationDirection;
};

#endif // BOUNDINGBOXPOINT_H
