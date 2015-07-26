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
#include "model/SketchItemRectangle.h"
#include "model/SketchItemEllipse.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentBlueprint(nullptr),
    mCurrentTool(nullptr),
    mCreatingItem(nullptr)
{
    mUi->setupUi(this);

    mScene = new QGraphicsScene(this);
    mUi->canvas->setScene(mScene);

    connect(mUi->canvas, &CanvasView::signalMousePressEvent, this, &MainWindow::onCanvasMousePressEvent);
    connect(mUi->canvas, &CanvasView::signalMouseMoveEvent, this, &MainWindow::onCanvasMouseMoveEvent);
    connect(mUi->canvas, &CanvasView::signalMouseReleaseEvent, this, &MainWindow::onCanvasMouseReleaseEvent);
    connect(mScene, &QGraphicsScene::focusItemChanged, this, &MainWindow::onFocusItemChanged);

    initToolbar();

    Sketch* sketch = new Sketch();
    Page* p = new Page();
    p->addSketch(sketch);

    mCurrentBlueprint = new Blueprint();
    mCurrentBlueprint->addPage(p);
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

void MainWindow::onCanvasMousePressEvent(QPointF point)
{
    static uint id = 0;

    if (mCurrentTool->getType() == Tool::Type::RECTANGLE) {
        SketchItemRectangle* sketchItem = new SketchItemRectangle(point.x(), point.y());
        sketchItem->boundBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, QPointF(-100.0f, -50.0f));
        sketchItem->name = QString("Rectangle #%1").arg(id++);

        mScene->addItem(sketchItem->getGraphicsItem());
        mCreatingItem = sketchItem;
        mCreatingLastPosition = point;

    } else  if (mCurrentTool->getType() == Tool::Type::ELLIPSE) {
        SketchItemEllipse* sketchItem = new SketchItemEllipse(point.x(), point.y());
        sketchItem->boundBoxPointMoved(BoundingBoxPoint::BOTTOM, QPointF(0.0f, -100.0f));
        sketchItem->boundBoxPointMoved(BoundingBoxPoint::RIGHT, QPointF(-50.0f, 0.0f));
        sketchItem->boundBoxPointMoved(BoundingBoxPoint::LEFT, QPointF(50.0f, 0.0f));
        sketchItem->name = QString("Ellipse #%1").arg(id++);

        mScene->addItem(sketchItem->getGraphicsItem());
        mCreatingItem = sketchItem;
        mCreatingLastPosition = point;
    }

}

void MainWindow::onCanvasMouseMoveEvent(QPointF point)
{
    if (mCreatingItem != nullptr) {
        QPointF delta = point - mCreatingLastPosition;
        mCreatingLastPosition = point;
        mCreatingItem->boundBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, delta);
    }
}

void MainWindow::onCanvasMouseReleaseEvent(QPointF point)
{
    mCreatingItem = nullptr;
    mCreatingLastPosition = QPointF(0.0f, 0.0f);
}

void MainWindow::onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason)
{
    QVariant itemVariant = newFocusItem->data(0);
    SketchItem* item = static_cast<SketchItem*>(itemVariant.value<void *>());
    if (item != nullptr){
        qDebug() << "Focus item is now " << item->name;
    }
}

