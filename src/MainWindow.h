#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Tool.h"

namespace Ui {
class MainWindow;
}

class QGraphicsScene;

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

private:
    void initToolbar();

private:
    Ui::MainWindow* mUi;
    QGraphicsScene* mScene;
    Tool* mCurrentTool;
    QList<Tool*> mTools;
};

#endif // MAINWINDOW_H
