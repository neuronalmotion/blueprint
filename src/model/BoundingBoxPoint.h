#ifndef BOUNDINGBOXPOINT_H
#define BOUNDINGBOXPOINT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

namespace blueprint {

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

    const int HANDLE_SIZE = 10;

    BoundingBoxPoint(BoundingBox* parent, TranslationDirection direction);
    ~BoundingBoxPoint();

    TranslationDirection getTranslationDirection() const;
    inline BoundingBox* getParentBoundingBox() const { return mParentBoundingBox; }

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

private:
    QPointF restrictPosition(const QPointF& newPosition);

private:
    BoundingBox* mParentBoundingBox;
    TranslationDirection mTranslationDirection;
};

}

#endif // BOUNDINGBOXPOINT_H
