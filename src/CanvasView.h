#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>
#include <QPointF>

#include "Tool.h"
#include "model/Shape.h"

namespace blueprint {
class Canvas;
class TreeItem;
class Shape;
}

class CanvasView : public QGraphicsView
{
    Q_OBJECT
public:
    CanvasView(QWidget* parent = 0);
    ~CanvasView();

    void selectGraphicalItem(blueprint::TreeItem* item);

public slots:
    void setTool(Tool::Type toolType);
    void onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason);

protected:
    // from QGraphicsView
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void fitView();

protected:
    Tool::Type mCurrentTool;
    blueprint::TreeItem* mSelectedGraphicalItem;
    blueprint::Canvas* mCurrentCanvas;
    blueprint::Shape* mCreatingShape;
    QPointF mCreatingLastPosition;
    float mZoomFactor;
};

#endif // CANVASVIEW_H
