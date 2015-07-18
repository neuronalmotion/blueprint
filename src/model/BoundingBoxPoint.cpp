#include "BoundingBoxPoint.h"

#include <QBrush>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>

BoundingBoxPoint::BoundingBoxPoint(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
    setBrush(QBrush(Qt::blue));
}

BoundingBoxPoint::~BoundingBoxPoint()
{

}

void BoundingBoxPoint::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    QGraphicsRectItem::dragMoveEvent(event);
    qDebug() << "Moving bounding box point" << event->pos();
}

