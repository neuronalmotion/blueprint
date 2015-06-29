#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QDebug>

#include "CanvasView.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentTool(nullptr)
{
    mUi->setupUi(this);
    mScene = new QGraphicsScene(this);

    mUi->canvas->setScene(mScene);
    connect(mUi->canvas, &CanvasView::signalMouseReleaseEvent, this, &MainWindow::onCanvasMouseReleaseEvent);
    initToolbar();
}

MainWindow::~MainWindow()
{
    delete mUi;
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
    mTools.append(new Tool(Tool::Type::CIRCLE, mUi->actionCircle));
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
    mScene->addEllipse(point.x(), point.y(), 50, 50);
}

