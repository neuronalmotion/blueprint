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

void CanvasView::resizeEvent(QResizeEvent* event)
{
    fitView();
}

void CanvasView::showEvent(QShowEvent* event)
{
    fitView();
}

void CanvasView::fitView()
{
    const QRectF rect = QRectF(0,0, this->width(), this->height());
    fitInView(rect, Qt::KeepAspectRatio);
    setSceneRect(rect);
}


