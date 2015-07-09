#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>

#include "CanvasView.h"
#include "model/Blueprint.h"
#include "model/Page.h"
#include "model/Sketch.h"
#include "model/SketchItemBezier.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentBlueprint(nullptr),
    mCurrentTool(nullptr)
{
    mUi->setupUi(this);
    mScene = new QGraphicsScene(this);

    mUi->canvas->setScene(mScene);
    connect(mUi->canvas, &CanvasView::signalMouseReleaseEvent, this, &MainWindow::onCanvasMouseReleaseEvent);
    initToolbar();

    SketchItemBezier* item = new SketchItemBezier();
    item->addPath(QPointF(30, 0), QPointF(50, 20), QPointF(50, 50));
    item->addPath(QPointF(50, 80), QPointF(30, 100), QPointF(0, 100));
    item->addPath(QPointF(-30, 100), QPointF(-40, 80), QPointF(-40, 50));
    item->addPath(QPointF(-40, 20), QPointF(-30, 0), QPointF(0, 0));
    item->closePath();

    Sketch* sketch = new Sketch();
    sketch->addSketchItem(item);
    Page* p = new Page();
    p->addSketch(sketch);

    mCurrentBlueprint = new Blueprint();
    mCurrentBlueprint->addPage(p);

    mScene->addItem(item->getGraphicsItem());
}

MainWindow::~MainWindow()
{
    delete mUi;
    if (mCurrentBlueprint) {
        delete mCurrentBlueprint;
    }
    mCurrentBlueprint = nullptr;
    mCurrentTool = nullptr;
    for(auto tool : mTools) {
        delete tool;
        tool = nullptr;
    }
    mTools.clear();
}

void MainWindow::initToolbar()
{
    mTools.append(new Tool(Tool::Type::SELECTION, mUi->actionSelection));
    mTools.append(new Tool(Tool::Type::ELLIPSE, mUi->actionCircle));
    mTools.append(new Tool(Tool::Type::LINE, mUi->actionLine));
    mTools.append(new Tool(Tool::Type::RECTANGLE, mUi->actionRectangle));
    setTool(Tool::Type::SELECTION);

    for(Tool* tool : mTools) {
        mUi->toolBar->addAction(tool->getAction());
        connect(tool->getAction(), &QAction::triggered, [this, tool]() { this->setTool(tool->getType()); });
    }
}

void MainWindow::setTool(Tool::Type toolType)
{
    qDebug() << "Selecting tool" << toolType;
    if (mCurrentTool) {
        mCurrentTool->setChecked(false);
    }
    for(Tool* tool : mTools) {
        if (tool->getType() == toolType) {
            tool->setChecked(true);
            mCurrentTool = tool;
            break;
        }
    }
}

void MainWindow::onCanvasMouseReleaseEvent(QPointF point)
{
//    QPainterPath path(point);
//    path.cubicTo(QPointF(0, 0), QPointF(100, 100), point);
//    QGraphicsPathItem* item = new QGraphicsPathItem(path);
//    mScene->addItem(item);
}

