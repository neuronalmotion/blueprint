#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QWidget>
#include <QKeyEvent>

#include "CanvasView.h"
#include "model/Blueprint.h"
#include "model/Page.h"
#include "model/Canvas.h"
#include "model/Shape.h"
#include "model/ShapeRectangle.h"
#include "model/ShapeEllipse.h"
#include "model/BezierPoint.h"
#include "model/BezierControlPoint.h"
#include "model/ShapeModel.h"

using namespace blueprint;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentBlueprint(nullptr)
{
    mUi->setupUi(this);
    resize(1200,700);

    initToolbar();

    // Warning! The scene must be owned by MainWindow
    // and deleted manually (no Qt parenting).
    // All drawn TreeItems (Shapes) are derived classes
    // of QGraphicsItem, which would delete our items before us
    mScene = new QGraphicsScene();
    ShapeModel* model = ShapeModel::instance();
    mUi->canvas->setScene(mScene);
    mUi->treeView->setModel(model);

    mCurrentBlueprint = new Blueprint();

    Page* p1 = new Page();
    p1->setName("Page 1");

    Canvas* c1 = new Canvas(p1, 0, 0);
    c1->setName("Canvas 1");
    mScene->addItem(c1->graphicsItem());

    Canvas* c2 = new Canvas(p1, 450, 0);
    c2->setName("Canvas 2");
    mScene->addItem(c2->graphicsItem());

    model->setRootItem(p1);
    model->addItem(c1, p1);
    model->addItem(c2, p1);

    initSignalSlots();
}

MainWindow::~MainWindow()
{
    // remove items *before* cleaning treeitem
    for (auto item : mScene->items()) {
        mScene->removeItem(item);
    }
    delete mCurrentBlueprint;
    delete ShapeModel::instance();
    delete mUi;
    qDeleteAll(mTools);
}

void MainWindow::initToolbar()
{
    mTools.append(new Tool(Tool::Type::SELECTION, mUi->actionSelection));
    mTools.append(new Tool(Tool::Type::ELLIPSE, mUi->actionCircle));
    mTools.append(new Tool(Tool::Type::LINE, mUi->actionLine));
    mTools.append(new Tool(Tool::Type::RECTANGLE, mUi->actionRectangle));
    mUi->canvas->setTool(Tool::Type::SELECTION);

    for(Tool* tool : mTools) {
        mUi->toolBar->addAction(tool->getAction());
        connect(tool->getAction(), &QAction::triggered, [this, tool]() {
            setTool(tool->getType());
        });
    }
}

void MainWindow::initSignalSlots()
{
    ShapeModel* model = ShapeModel::instance();
    connect(model, &ShapeModel::shapeAdded,
            [this](Shape* shape) {
        mUi->statusBar->showMessage(QString("Created shape ") + shape->name());
    });

    connect(mUi->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [this, model] (const QModelIndex& current,const QModelIndex&/* previous*/) {
        Shape* item = model->itemFromIndex(current);
        if (!item->parentShape()) {
            return;
        }
        model->shapeSelected(item);
    });

    connect(model, &ShapeModel::shapeSelected,
            [this](Shape* shape) {
        mUi->treeView->selectionModel()->select(*shape->modelIndex(),
                                                QItemSelectionModel::ClearAndSelect);
    });

    connect(model, &ShapeModel::shapeRemoved,
            [this](Shape* shape) {
        mUi->statusBar->showMessage(QString("Deleted shape ") + shape->name());
        mScene->removeItem(shape->graphicsItem());
        delete shape;
    });

}

void MainWindow::setTool(Tool::Type toolType)
{
    qDebug() << "Selecting tool" << toolType;
    for(Tool* tool : mTools) {
        if (tool->getType() == toolType) {
            tool->setChecked(true);
        } else {
            tool->setChecked(false);
        }
    }
    mUi->canvas->setTool(toolType);
}
