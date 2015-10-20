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
class Shape;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void initToolbar();
    void initSignalSlots();
    void saveShapeToImage(blueprint::Shape& shape, const QString& filepath);

private slots:
    void setTool(blueprint::Tool::Type toolType);
    void exportShapeToImage();
    void saveFile();
    void loadFile();

private:
    Ui::MainWindow* mUi;
    QGraphicsScene* mScene;
    blueprint::Blueprint* mCurrentBlueprint;
    QList<blueprint::Tool*> mTools;
};

#endif // MAINWINDOW_H
