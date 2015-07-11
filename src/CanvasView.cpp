#include "CanvasView.h"

#include <QMouseEvent>

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent)
{

}

CanvasView::~CanvasView()
{

}

void CanvasView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    QPointF localPos = QGraphicsView::mapToScene(event->pos());
    emit signalMouseReleaseEvent(localPos);
}


