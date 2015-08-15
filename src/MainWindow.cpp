#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>

#include "CanvasView.h"
#include "model/Blueprint.h"
#include "model/Page.h"
#include "model/Canvas.h"
#include "model/ShapeBezier.h"
#include "model/ShapeRectangle.h"
#include "model/ShapeEllipse.h"
#include "model/BezierPoint.h"
#include "model/BezierControlPoint.h"
#include "model/GraphicalItem.h"
#include "model/GraphicalModel.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentBlueprint(nullptr),
    mCurrentTool(nullptr),
    mCreatingShape(nullptr),
    mSelectedShape(nullptr)
{
    mUi->setupUi(this);

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

    Page* p1 = new Page(mCurrentBlueprint);
    p1->setName("Page 1");
    mCurrentBlueprint->appendChild(p1);

    Canvas* c1 = new Canvas(p1, 0, 0);
    c1->setName("Canvas 1");
    c1->setBorderColor(QColor(10, 10, 10));
    c1->setBackgroundColor(QColor(225, 225, 225));
    mScene->addItem(c1->getGraphicsItem());
    p1->appendChild(c1);
    mCurrentCanvas = c1;

    mModel = new GraphicalModel(mCurrentBlueprint, this);
    mUi->treeView->setModel(mModel);

    // FIXME this code is so fragile that it should be deleted very quickly
    // it works only if the parent Canvas is selected in the treeview...
    connect(mModel, &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
        GraphicalItem* parentItem = mModel->graphicalItemFromIndex(parent);
        GraphicalItem* childItem = parentItem->child(first);
        mUi->statusBar->showMessage(QString("Created item ") + childItem->name());
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
        ShapeRectangle* shape = new ShapeRectangle(mCurrentCanvas, point.x(), point.y());
        //sketchItem->boundBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, QPointF(-100.0f, -50.0f));
        shape->setName(QString("Rectangle #%1").arg(id++));

        mScene->addItem(shape->getGraphicsItem());
        mCreatingShape = shape;
        mCreatingLastPosition = point;
        QModelIndex index = mUi->treeView->selectionModel()->currentIndex();
        mModel->addGraphicalItem(mCreatingShape, mCurrentCanvas, index);

    } else  if (mCurrentTool->getType() == Tool::Type::ELLIPSE) {
        ShapeEllipse* shape = new ShapeEllipse(mCurrentCanvas, point.x(), point.y());
        //sketchItem->boundBoxPointMoved(BoundingBoxPoint::BOTTOM, QPointF(0.0f, -100.0f));
        //sketchItem->boundBoxPointMoved(BoundingBoxPoint::RIGHT, QPointF(-50.0f, 0.0f));
        //sketchItem->boundBoxPointMoved(BoundingBoxPoint::LEFT, QPointF(50.0f, 0.0f));
        shape->setName(QString("Ellipse #%1").arg(id++));

        mScene->addItem(shape->getGraphicsItem());
        mCreatingShape = shape;
        mCreatingLastPosition = point;
        QModelIndex index = mUi->treeView->selectionModel()->currentIndex();
        mModel->addGraphicalItem(mCreatingShape, mCurrentCanvas, index);
    }

}

void MainWindow::onCanvasMouseMoveEvent(QPointF point)
{
    if (mCreatingShape != nullptr) {
        QPointF delta = point - mCreatingLastPosition;
        mCreatingLastPosition = point;
        //mCreatingItem->boundBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, delta);
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

    if (mSelectedShape != nullptr) {
        mSelectedShape->setIsSelected(false);
    }

    // SKETCH_ITEM_BEZIER
    itemVariant = newFocusItem->data(Shape::Type::SHAPE_BEZIER);
    ShapeBezier* sketchItemBezier = static_cast<ShapeBezier*>(itemVariant.value<void *>());
    if (sketchItemBezier != nullptr) {
        mSelectedShape = sketchItemBezier;

        //mSelectedSketchItem->setEditMode(SketchItem::EditMode::BOUNDING_BOX);
        mSelectedShape->setIsSelected(true);

        qDebug() << "Focus item is now " << mSelectedShape->name();
    }

    // BOUNDING_BOX_POINT
    itemVariant = newFocusItem->data(Shape::Type::BOUNDING_BOX_POINT);
    BoundingBoxPoint* boundingBoxPoint = static_cast<BoundingBoxPoint*>(itemVariant.value<void *>());
    if (boundingBoxPoint != nullptr) {
        mSelectedShape = boundingBoxPoint->getParentBoundingBox()->getParentSketchItem();
        mSelectedShape->getGraphicsItem()->setFocus();
        qDebug() << "Focus item is now " << mSelectedShape->name() << " (from boundingBoxPoint)";
    }

    // BEZIER_POINT
    itemVariant = newFocusItem->data(Shape::Type::BEZIER_POINT);
    BezierPoint* bezierPoint = static_cast<BezierPoint*>(itemVariant.value<void *>());
    if (bezierPoint != nullptr) {
        mSelectedShape = bezierPoint->getParentSketchItemBezier();
        mSelectedShape->getGraphicsItem()->setFocus();
        qDebug() << "Focus item is now " << mSelectedShape->name() << " (from bezier point)";
    }

    // BEZIER_CONTROL_POINT
    itemVariant = newFocusItem->data(Shape::Type::BEZIER_CONTROL_POINT);
    BezierControlPoint* bezierControlPoint = static_cast<BezierControlPoint*>(itemVariant.value<void *>());
    if (bezierControlPoint != nullptr) {
        mSelectedShape = bezierControlPoint->getParentSketchItemBezier();
        mSelectedShape->getGraphicsItem()->setFocus();
        qDebug() << "Focus item is now " << mSelectedShape->name() << " (from bezier control point)";
    }
}

void MainWindow::onCanvasKeyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {

        if (mSelectedShape != nullptr) {
            ShapeBezier* sketchItemBezier = dynamic_cast<ShapeBezier*>(mSelectedShape);
            if (sketchItemBezier){
                sketchItemBezier->setEditMode(Shape::EditMode::BEZIER);
            }
        }
    }
}

void MainWindow::onCanvasKeyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        ShapeBezier* sketchItemBezier = dynamic_cast<ShapeBezier*>(mSelectedShape);
        if (sketchItemBezier){
            sketchItemBezier->setEditMode(Shape::EditMode::BOUNDING_BOX);
        }
    }
}
