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
    void signalKeyPressEvent(QKeyEvent *event);
    void signalKeyReleaseEvent(QKeyEvent *event);

protected: // from QGraphicsView
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

protected:
    void fitView();

    float mZoomfactor;
};

#endif // CANVASVIEW_H
