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
    void signalMouseReleaseEvent(QPointF point);

protected:
    void mouseReleaseEvent(QMouseEvent* event);
};

#endif // CANVASVIEW_H
