#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>
#include <QPointF>

#include "Tool.h"
#include "model/Shape.h"

namespace blueprint {
class Canvas;
class Shape;
class Tool;
class TreeItem;
}


class CanvasView : public QGraphicsView
{
    Q_OBJECT
public:
    CanvasView(QWidget* parent = 0);
    ~CanvasView();

    void setScene(QGraphicsScene *scene);

public slots:
    void setTool(blueprint::Tool::Type toolType);
    void onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason);
    void shapePropertiesChanged(blueprint::Shape* shape);

protected:
    // from QGraphicsView
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    blueprint::Shape* shapeFromScenePoint(const QPointF& point);

private:
    blueprint::Tool::Type mCurrentTool;
    blueprint::Shape* mCreatingShape;
    QPointF mCreatingLastPosition;
    float mZoomFactor;
};

#endif // CANVASVIEW_H
