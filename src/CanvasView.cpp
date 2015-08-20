#include "CanvasView.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsScene>

#include "Tool.h"
#include "model/BezierControlPoint.h"
#include "model/BezierPoint.h"
#include "model/Canvas.h"
#include "model/Shape.h"
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
    mSelectedShape(nullptr),
    mZoomFactor(1.0f)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);

    connect(scene, &QGraphicsScene::focusItemChanged, this, &CanvasView::onFocusItemChanged);
    connect(TreeModel::instance(), &TreeModel::selectionsChanged, this, &CanvasView::selectionsChanged);
}

CanvasView::~CanvasView()
{
}

void CanvasView::selectionsChanged(const QModelIndex& parent, int first, int last)
{
    TreeModel* model = TreeModel::instance();
    blueprint::Shape* item = static_cast<blueprint::Shape*>(model->itemFromParentIndex(parent, first));
    Q_ASSERT(item);

    if (item->itemType() == TreeItem::ItemType::CANVAS) {
        mCurrentCanvas = static_cast<Canvas*>(item);
    }

    if (item == mSelectedShape) {
       return;
    }
    if (mSelectedShape) {
       mSelectedShape->setSelected(false);
    }
    item->setSelected(true);
    mSelectedShape = item;
    qDebug() << "Selected item " << mSelectedShape->name();
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
        shape->setParentItem(mCurrentCanvas);
        mCreatingShape = shape;
        mCreatingLastPosition = point;
        model->addItem(mCreatingShape, mCurrentCanvas);

    } else  if (mCurrentTool == Tool::Type::ELLIPSE) {
        ShapeEllipse* shape = new ShapeEllipse(mCurrentCanvas, relPoint.x(), relPoint.y());
        shape->setName(QString("Ellipse #%1").arg(id++));
        shape->setParentItem(mCurrentCanvas);
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
        if (mSelectedShape) {
            blueprint::Shape* sketchItemBezier = dynamic_cast<blueprint::Shape*>(mSelectedShape);
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
        blueprint::Shape* sketchItemBezier = dynamic_cast<blueprint::Shape*>(mSelectedShape);
        if (sketchItemBezier){
            sketchItemBezier->setEditMode(blueprint::Shape::EditMode::BOUNDING_BOX);
        }
    }
    return QGraphicsView::keyReleaseEvent(event);
}

void CanvasView::onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* oldFocusItem, Qt::FocusReason reason)
{
    QVariant itemVariant = newFocusItem->data(blueprint::Shape::ShapeType::SHAPE);
    blueprint::Shape* shape = static_cast<blueprint::Shape*>(itemVariant.value<void *>());
    if (!shape) {
        return;
    }
    QModelIndex index = (QModelIndex)(*shape->parentTreeItem()->modelIndex());
    int shapeIndex = shape->parentTreeItem()->indexOf(shape);
    TreeModel::instance()->selectionsChanged(index, shapeIndex, shapeIndex);
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


