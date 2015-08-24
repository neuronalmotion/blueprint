#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>

#include "Tool.h"

namespace Ui {
class MainWindow;
}

namespace blueprint {
class Blueprint;
class Canvas;
class Tool;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void initToolbar();

private slots:
    void setTool(blueprint::Tool::Type toolType);

private:
    Ui::MainWindow* mUi;
    QGraphicsScene* mScene;
    blueprint::Blueprint* mCurrentBlueprint;
    QList<blueprint::Tool*> mTools;
};

#endif // MAINWINDOW_H
