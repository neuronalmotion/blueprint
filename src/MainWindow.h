#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QGraphicsItem>
#include <QKeyEvent>

#include "Tool.h"

namespace Ui {
class MainWindow;
}

namespace blueprint {
class Blueprint;
class Canvas;
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
    void setTool(Tool::Type toolType);

private:
    Ui::MainWindow* mUi;
    blueprint::Blueprint* mCurrentBlueprint;
    QList<Tool*> mTools;
};

#endif // MAINWINDOW_H
