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

protected: // from QGraphicsView
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

protected:
    void fitView();
};

#endif // CANVASVIEW_H
