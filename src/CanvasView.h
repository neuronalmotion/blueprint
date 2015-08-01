#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>

class CanvasView : public QGraphicsView
{
    Q_OBJECT
public:
    CanvasView(QWidget* parent = 0);
    ~CanvasView();

signals:
    void signalMousePressEvent(QPointF point);
    void signalMouseMoveEvent(QPointF point);
    void signalMouseReleaseEvent(QPointF point);

protected: // from QGraphicsView
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

protected:
    void fitView();

    float mZoomfactor;
};

#endif // CANVASVIEW_H
