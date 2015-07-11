#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QGraphicsItem>

#include "Tool.h"

namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class Blueprint;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void setTool(Tool::Type toolType);

private slots:
    void onCanvasMouseReleaseEvent(QPointF point);
    void onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason);

private:
    void initToolbar();

private:
    Ui::MainWindow* mUi;
    QGraphicsScene* mScene;
    Blueprint* mCurrentBlueprint;
    Tool* mCurrentTool;
    QList<Tool*> mTools;
};

#endif // MAINWINDOW_H
