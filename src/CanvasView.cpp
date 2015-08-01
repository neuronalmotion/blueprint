#include "CanvasView.h"

#include <QDebug>
#include <QMouseEvent>

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent),
      mZoomfactor(1.0f)
{

}

CanvasView::~CanvasView()
{

}

void CanvasView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    QPointF localPos = QGraphicsView::mapToScene(event->pos());
    emit signalMousePressEvent(localPos);
}

void CanvasView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF localPos = QGraphicsView::mapToScene(event->pos());
    emit signalMouseMoveEvent(localPos);
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    QPointF localPos = QGraphicsView::mapToScene(event->pos());
    emit signalMouseReleaseEvent(localPos);
}

void CanvasView::wheelEvent(QWheelEvent *event)
{
    mZoomfactor +=  0.1f * (event->delta() / abs(event->delta()));
    mZoomfactor = qBound(0.2f, mZoomfactor, 2.0f);
    fitView();
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
    float viewWidth = this->width() * mZoomfactor;
    float viewHeight = this->height() * mZoomfactor;
    const QRectF rect = QRectF((this->width() - viewWidth) / 2.0f,
                               (this->height() - viewHeight) / 2.0f,
                               viewWidth,
                               viewHeight);
    fitInView(rect, Qt::KeepAspectRatio);
    setSceneRect(rect);
}


