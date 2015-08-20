#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QGraphicsItem>
#include <QKeyEvent>

#include "model/Shape.h"
#include "Tool.h"

namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class Blueprint;
class Canvas;
class TreelModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void setTool(Tool::Type toolType);

private slots:
    void onCanvasMousePressEvent(QPointF point);
    void onCanvasMouseMoveEvent(QPointF point);
    void onCanvasMouseReleaseEvent(QPointF point);
    void onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason);
    void onCanvasKeyPressEvent(QKeyEvent *event);
    void onCanvasKeyReleaseEvent(QKeyEvent *event);

private:
    void initToolbar();
    void selectGraphicalItem(TreeItem* item);

private:
    Ui::MainWindow* mUi;
    QGraphicsScene* mScene;
    Blueprint* mCurrentBlueprint;
    TreelModel* mModel;
    Canvas* mCurrentCanvas;
    Tool* mCurrentTool;
    QList<Tool*> mTools;
    Shape* mCreatingShape;
    QPointF mCreatingLastPosition;
    TreeItem* mSelectedGraphicalItem;
};

#endif // MAINWINDOW_H
