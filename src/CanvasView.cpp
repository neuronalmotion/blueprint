#include "CanvasView.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsScene>

#include "Tool.h"
#include "model/BezierControlPoint.h"
#include "model/BezierPoint.h"
#include "model/Canvas.h"
#include "model/ShapeRectangle.h"
#include "model/ShapeEllipse.h"
#include "model/TreeItem.h"
#include "model/TreeModel.h"

using namespace blueprint;

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent),
    mCurrentTool(Tool::Type::SELECTION),
    mCreatingShape(nullptr),
    mCurrentCanvas(nullptr),
    mSelectedGraphicalItem(nullptr),
    mZoomFactor(1.0f)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);

    connect(scene, &QGraphicsScene::focusItemChanged, this, &CanvasView::onFocusItemChanged);
}

CanvasView::~CanvasView()
{
}

void CanvasView::selectGraphicalItem(blueprint::TreeItem* item)
{
    if (item == mSelectedGraphicalItem) {
       return;
    }
    if (mSelectedGraphicalItem) {
       mSelectedGraphicalItem->setSelected(false);
    }
    scene()->clearFocus();
    item->setSelected(true);
    mSelectedGraphicalItem = item;
    qDebug() << "Selected item " << mSelectedGraphicalItem->name();
}

void CanvasView::setTool(Tool::Type toolType)
{
    mCurrentTool = toolType;
}

void CanvasView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    QPointF point = QGraphicsView::mapToScene(event->pos());
    TreeModel* model = TreeModel::instance();
    static uint id = 0;
    if (!mCurrentCanvas) {
        return;
    }
    QPointF relPoint(point.x() - mCurrentCanvas->pos().x(),
                                point.y() - mCurrentCanvas->pos().y());

    if (mCurrentTool == Tool::Type::RECTANGLE) {
        ShapeRectangle* shape = new ShapeRectangle(mCurrentCanvas, relPoint.x(), relPoint.y());
        shape->setName(QString("Rectangle #%1").arg(id++));

        scene()->addItem(shape);
        mCreatingShape = shape;
        mCreatingLastPosition = point;
        model->addItem(mCreatingShape, mCurrentCanvas);

    } else  if (mCurrentTool == Tool::Type::ELLIPSE) {
        ShapeEllipse* shape = new ShapeEllipse(mCurrentCanvas, relPoint.x(), relPoint.y());
        shape->setName(QString("Ellipse #%1").arg(id++));

        scene()->addItem(shape);
        mCreatingShape = shape;
        mCreatingLastPosition = point;
        model->addItem(mCreatingShape, mCurrentCanvas);
    }
}

void CanvasView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF point = QGraphicsView::mapToScene(event->pos());
    if (mCreatingShape) {
        QPointF delta = point - mCreatingLastPosition;
        mCreatingLastPosition = point;
        //TODO: boundingBoxPointMoved
    }
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    mCreatingShape = nullptr;
    mCreatingLastPosition = QPointF(0.0f, 0.0f);
}

void CanvasView::wheelEvent(QWheelEvent *event)
{
    mZoomFactor +=  0.1f * (event->delta() / abs(event->delta()));
    mZoomFactor = qBound(0.2f, mZoomFactor, 2.0f);
    fitView();
}

void CanvasView::resizeEvent(QResizeEvent* event)
{
    fitView();
}

void CanvasView::showEvent(QShowEvent* event)
{
    fitView();
}

void CanvasView::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat() && event->key() == Qt::Key_Space) {
        if (mSelectedGraphicalItem) {
            blueprint::Shape* sketchItemBezier = dynamic_cast<blueprint::Shape*>(mSelectedGraphicalItem);
            if (sketchItemBezier){
                sketchItemBezier->setEditMode(blueprint::Shape::EditMode::BEZIER);
            }
        }
    }
    return QGraphicsView::keyPressEvent(event);
}

void CanvasView::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat() && event->key() == Qt::Key_Space) {
        blueprint::Shape* sketchItemBezier = dynamic_cast<blueprint::Shape*>(mSelectedGraphicalItem);
        if (sketchItemBezier){
            sketchItemBezier->setEditMode(blueprint::Shape::EditMode::BOUNDING_BOX);
        }
    }
    return QGraphicsView::keyReleaseEvent(event);
}

void CanvasView::onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason)
{
    QVariant itemVariant;

    qDebug() << "\n onFocusItemChanged()";

    if (mSelectedGraphicalItem) {
        mSelectedGraphicalItem->setSelected(false);
    }

    // CANVAS
    itemVariant = newFocusItem->data(blueprint::Shape::ShapeType::CANVAS);
    Canvas* canvas = static_cast<Canvas*>(itemVariant.value<void *>());
    if (canvas) {
        mSelectedGraphicalItem = canvas;
        mCurrentCanvas = canvas;
        mSelectedGraphicalItem->setSelected(true);
        qDebug() << "Focus item is now " << mSelectedGraphicalItem->name();
        return;
    }

    // SHAPE_BEZIER
    itemVariant = newFocusItem->data(blueprint::Shape::ShapeType::SHAPE_BEZIER);
    blueprint::Shape* sketchItemBezier = static_cast<blueprint::Shape*>(itemVariant.value<void *>());
    if (sketchItemBezier) {
        mSelectedGraphicalItem = sketchItemBezier;
        mSelectedGraphicalItem->setSelected(true);
//        mUi->treeView->selectionModel()->select(*mSelectedGraphicalItem->modelIndex(),
//                                                QItemSelectionModel::ClearAndSelect);
        qDebug() << "Focus item is now " << mSelectedGraphicalItem->name();
        return;
    }

    // BOUNDING_BOX_POINT
    itemVariant = newFocusItem->data(blueprint::Shape::ShapeType::BOUNDING_BOX_POINT);
    BoundingBoxPoint* boundingBoxPoint = static_cast<BoundingBoxPoint*>(itemVariant.value<void *>());
    if (boundingBoxPoint) {
        blueprint::Shape* item = boundingBoxPoint->getParentBoundingBox()->getParentSketchItem();
        item->setFocus();
        mSelectedGraphicalItem = item;
        qDebug() << "Focus item is now " << item->name() << " (from boundingBoxPoint)";
        return;
    }

    // BEZIER_POINT
    itemVariant = newFocusItem->data(blueprint::Shape::ShapeType::BEZIER_POINT);
    BezierPoint* bezierPoint = static_cast<BezierPoint*>(itemVariant.value<void *>());
    if (bezierPoint) {
        blueprint::Shape* item = bezierPoint->getParentSketchItemBezier();
        item->setFocus();
        mSelectedGraphicalItem = item;
        qDebug() << "Focus item is now " << item->name() << " (from bezier point)";
    }

    // BEZIER_CONTROL_POINT
    itemVariant = newFocusItem->data(blueprint::Shape::ShapeType::BEZIER_CONTROL_POINT);
    BezierControlPoint* bezierControlPoint = static_cast<BezierControlPoint*>(itemVariant.value<void *>());
    if (bezierControlPoint) {
        blueprint::Shape* item = bezierControlPoint->getParentSketchItemBezier();
        item->setFocus();
        mSelectedGraphicalItem = item;
        qDebug() << "Focus item is now " << item->name() << " (from bezier control point)";
    }
}

void CanvasView::fitView()
{
    float viewWidth = this->width() * mZoomFactor;
    float viewHeight = this->height() * mZoomFactor;
    const QRectF rect = QRectF((this->width() - viewWidth) / 2.0f,
                               (this->height() - viewHeight) / 2.0f,
                               viewWidth,
                               viewHeight);
    fitInView(rect, Qt::KeepAspectRatio);
    setSceneRect(rect);
}


