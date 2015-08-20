#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>

#include "CanvasView.h"
#include "model/Blueprint.h"
#include "model/Page.h"
#include "model/Canvas.h"
#include "model/Shape.h"
#include "model/ShapeRectangle.h"
#include "model/ShapeEllipse.h"
#include "model/BezierPoint.h"
#include "model/BezierControlPoint.h"
#include "model/TreeItem.h"
#include "model/TreeModel.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentBlueprint(nullptr),
    mCurrentTool(nullptr),
    mCreatingShape(nullptr),
    mSelectedGraphicalItem(nullptr)
{
    mUi->setupUi(this);
    resize(1200,700);

    mScene = new QGraphicsScene(this);
    mUi->canvas->setScene(mScene);

    connect(mUi->canvas, &CanvasView::signalMousePressEvent, this, &MainWindow::onCanvasMousePressEvent);
    connect(mUi->canvas, &CanvasView::signalMouseMoveEvent, this, &MainWindow::onCanvasMouseMoveEvent);
    connect(mUi->canvas, &CanvasView::signalMouseReleaseEvent, this, &MainWindow::onCanvasMouseReleaseEvent);
    connect(mScene, &QGraphicsScene::focusItemChanged, this, &MainWindow::onFocusItemChanged);
    connect(mUi->canvas, &CanvasView::signalKeyPressEvent, this, &MainWindow::onCanvasKeyPressEvent);
    connect(mUi->canvas, &CanvasView::signalKeyReleaseEvent, this, &MainWindow::onCanvasKeyReleaseEvent);

    initToolbar();

    mCurrentBlueprint = new Blueprint();
    mCurrentBlueprint->setName("Blueprint");

    // Page 1
    Page* p1 = new Page(mCurrentBlueprint);
    p1->setName("Page 1");

    // Canvas 1
    Canvas* c1 = new Canvas(p1, 0, 0);
    c1->setName("Canvas 1");
    mScene->addItem(c1->getGraphicsItem());

    // Canvas 2
    Canvas* c2 = new Canvas(p1, 450, 0);
    c2->setName("Canvas 2");
    mScene->addItem(c2->getGraphicsItem());
    mCurrentCanvas = c1;

    // Common
    mModel = new TreelModel(mCurrentBlueprint, this);
    mUi->treeView->setModel(mModel);
    mModel->addItem(p1, mCurrentBlueprint);
    mModel->addItem(c1, p1, *p1->modelIndex());
    mModel->addItem(c2, p1, *p1->modelIndex());

    // FIXME this code is so fragile that it should be deleted very quickly
    // it works only if the parent Canvas is selected in the treeview...
    connect(mModel, &QAbstractItemModel::rowsInserted,
            [this](const QModelIndex& parent, int first, int /*last*/) {
        TreeItem* parentItem = mModel->itemFromIndex(parent);
        TreeItem* childItem = parentItem->child(first);
        mUi->statusBar->showMessage(QString("Created item ") + childItem->name());
    });

    connect(mUi->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [this] (const QModelIndex& current,const QModelIndex&/* previous*/) {
        TreeItem* item = mModel->itemFromIndex(current);
        selectGraphicalItem(item);
    });
}

MainWindow::~MainWindow()
{
    delete mUi;
    qDeleteAll(mTools);
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
        connect(tool->getAction(), &QAction::triggered, [this, tool]() {
            this->setTool(tool->getType()); });
    }
}

void MainWindow::selectGraphicalItem(TreeItem* item)
{
    if (item == mSelectedGraphicalItem) {
       return;
    }
    if (mSelectedGraphicalItem) {
       mSelectedGraphicalItem->setSelected(false);
    }
    mScene->clearFocus();
    item->setSelected(true);
    mSelectedGraphicalItem = item;
    qDebug() << "Selected item " << mSelectedGraphicalItem->name();
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

    QPointF relPoint(point.x() - mCurrentCanvas->getGraphicsItem()->pos().x(),
                                point.y() - mCurrentCanvas->getGraphicsItem()->pos().y());

    if (mCurrentTool->getType() == Tool::Type::RECTANGLE) {
        ShapeRectangle* shape = new ShapeRectangle(mCurrentCanvas, relPoint.x(), relPoint.y());
        shape->setName(QString("Rectangle #%1").arg(id++));

        mScene->addItem(shape->getGraphicsItem());
        mCreatingShape = shape;
        mCreatingLastPosition = point;
        QModelIndex index = mUi->treeView->selectionModel()->currentIndex();
        mModel->addItem(mCreatingShape, mCurrentCanvas, index);

    } else  if (mCurrentTool->getType() == Tool::Type::ELLIPSE) {
        ShapeEllipse* shape = new ShapeEllipse(mCurrentCanvas, relPoint.x(), relPoint.y());
        shape->setName(QString("Ellipse #%1").arg(id++));

        mScene->addItem(shape->getGraphicsItem());
        mCreatingShape = shape;
        mCreatingLastPosition = point;
        QModelIndex index = mUi->treeView->selectionModel()->currentIndex();
        mModel->addItem(mCreatingShape, mCurrentCanvas, index);
    }

}

void MainWindow::onCanvasMouseMoveEvent(QPointF point)
{
    if (mCreatingShape) {
        QPointF delta = point - mCreatingLastPosition;
        mCreatingLastPosition = point;
        //TODO: boundingBoxPointMoved
    }
}

void MainWindow::onCanvasMouseReleaseEvent(QPointF point)
{
    mCreatingShape = nullptr;
    mCreatingLastPosition = QPointF(0.0f, 0.0f);
}

void MainWindow::onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason)
{
    QVariant itemVariant;

    qDebug() << "\n onFocusItemChanged()";

    if (mSelectedGraphicalItem) {
        mSelectedGraphicalItem->setSelected(false);
    }

    // CANVAS
    itemVariant = newFocusItem->data(Shape::Type::CANVAS);
    Canvas* canvas = static_cast<Canvas*>(itemVariant.value<void *>());
    if (canvas) {
        mSelectedGraphicalItem = canvas;
        mCurrentCanvas = canvas;
        mSelectedGraphicalItem->setSelected(true);
        qDebug() << "Focus item is now " << mSelectedGraphicalItem->name();
        return;
    }

    // SHAPE_BEZIER
    itemVariant = newFocusItem->data(Shape::Type::SHAPE_BEZIER);
    Shape* sketchItemBezier = static_cast<Shape*>(itemVariant.value<void *>());
    if (sketchItemBezier) {
        mSelectedGraphicalItem = sketchItemBezier;
        mSelectedGraphicalItem->setSelected(true);
        mUi->treeView->selectionModel()->select(*mSelectedGraphicalItem->modelIndex(),
                                                QItemSelectionModel::ClearAndSelect);
        qDebug() << "Focus item is now " << mSelectedGraphicalItem->name();
        return;
    }

    // BOUNDING_BOX_POINT
    itemVariant = newFocusItem->data(Shape::Type::BOUNDING_BOX_POINT);
    BoundingBoxPoint* boundingBoxPoint = static_cast<BoundingBoxPoint*>(itemVariant.value<void *>());
    if (boundingBoxPoint) {
        Shape* item = boundingBoxPoint->getParentBoundingBox()->getParentSketchItem();
        item->getGraphicsItem()->setFocus();
        mSelectedGraphicalItem = item;
        qDebug() << "Focus item is now " << item->name() << " (from boundingBoxPoint)";
        return;
    }

    // BEZIER_POINT
    itemVariant = newFocusItem->data(Shape::Type::BEZIER_POINT);
    BezierPoint* bezierPoint = static_cast<BezierPoint*>(itemVariant.value<void *>());
    if (bezierPoint) {
        Shape* item = bezierPoint->getParentSketchItemBezier();
        item->getGraphicsItem()->setFocus();
        mSelectedGraphicalItem = item;
        qDebug() << "Focus item is now " << item->name() << " (from bezier point)";
    }

    // BEZIER_CONTROL_POINT
    itemVariant = newFocusItem->data(Shape::Type::BEZIER_CONTROL_POINT);
    BezierControlPoint* bezierControlPoint = static_cast<BezierControlPoint*>(itemVariant.value<void *>());
    if (bezierControlPoint) {
        Shape* item = bezierControlPoint->getParentSketchItemBezier();
        item->getGraphicsItem()->setFocus();
        mSelectedGraphicalItem = item;
        qDebug() << "Focus item is now " << item->name() << " (from bezier control point)";
    }
}

void MainWindow::onCanvasKeyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {

        if (mSelectedGraphicalItem) {
            Shape* sketchItemBezier = dynamic_cast<Shape*>(mSelectedGraphicalItem);
            if (sketchItemBezier){
                sketchItemBezier->setEditMode(Shape::EditMode::BEZIER);
            }
        }
    }
}

void MainWindow::onCanvasKeyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        Shape* sketchItemBezier = dynamic_cast<Shape*>(mSelectedGraphicalItem);
        if (sketchItemBezier){
            sketchItemBezier->setEditMode(Shape::EditMode::BOUNDING_BOX);
        }
    }
}
