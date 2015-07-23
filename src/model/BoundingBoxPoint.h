#ifndef BOUNDINGBOXPOINT_H
#define BOUNDINGBOXPOINT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

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
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

private:
    QPointF restrictPosition(const QPointF& newPosition);

private:
    BoundingBox* mParentBoundingBox;
    TranslationDirection mTranslationDirection;
};

#endif // BOUNDINGBOXPOINT_H
