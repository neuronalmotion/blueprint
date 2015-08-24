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

    void selectionsChanged(const QModelIndex& parent, int first, int last);

public slots:
    void setTool(blueprint::Tool::Type toolType);
    void onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason);

protected:
    // from QGraphicsView
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void fitView();

private:
    blueprint::Tool::Type mCurrentTool;
    blueprint::Shape* mSelectedShape;
    blueprint::Canvas* mCurrentCanvas;
    blueprint::Shape* mCreatingShape;
    QPointF mCreatingLastPosition;
    float mZoomFactor;
};

#endif // CANVASVIEW_H
