#include "CanvasView.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QTransform>

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
    mSelectedShape(nullptr),
    mCreatingShape(nullptr),
    mCreatingLastPosition(0, 0),
    mZoomFactor(1.0f)
{
    connect(TreeModel::instance(), &TreeModel::selectionsChanged, this, &CanvasView::selectionsChanged);
    connect(TreeModel::instance(), &TreeModel::propertiesChanged, this, &CanvasView::propertiesChanged);
}

CanvasView::~CanvasView()
{
}

void CanvasView::selectionsChanged(const QModelIndex& parent, int first, int /*last*/)
{
    TreeModel* model = TreeModel::instance();
    blueprint::Shape* item = static_cast<blueprint::Shape*>(model->itemFromParentIndex(parent, first));
    Q_ASSERT(item);

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

void CanvasView::propertiesChanged(const QModelIndex& parent, int first, int last)
{
    TreeModel* model = TreeModel::instance();
    blueprint::Shape* item = static_cast<blueprint::Shape*>(model->itemFromParentIndex(parent, first));
    Q_ASSERT(item);

    item->update();
}

void CanvasView::setTool(Tool::Type toolType)
{
    mCurrentTool = toolType;
}

void CanvasView::mousePressEvent(QMouseEvent *event)
{
    // ::: Selection tool actions :::
    if (mCurrentTool == Tool::Type::SELECTION) {

        // Only selection require to call QGraphicsView implementation
        QGraphicsView::mousePressEvent(event);

        return;
    }

    // ::: Shape Creation tool actions :::
    blueprint::Shape* shape = nullptr;

    // Search the parent item
    blueprint::Shape* shapeParent = nullptr;
    QPointF point = QGraphicsView::mapToScene(event->pos());
    TreeModel* model = TreeModel::instance();
    static uint id = 0;

    QGraphicsItem* graphicsItem = scene()->itemAt(point, QTransform());

    // Can't add a shape outside of another Shape (or a Canvas)
    if (graphicsItem == nullptr) {
          return;
    }

    // Position is always relative to direct parent
    shapeParent = dynamic_cast<blueprint::Shape*>(graphicsItem);
    QPointF relPoint(point.x() - shapeParent->posAbsolute().x(),  point.y() - shapeParent->posAbsolute().y());

    // Create the right Shape
    if (mCurrentTool == Tool::Type::RECTANGLE) {
        shape = new ShapeRectangle(shapeParent, relPoint.x(), relPoint.y());
        shape->setName(QString("Rectangle #%1").arg(id++));

    } else  if (mCurrentTool == Tool::Type::ELLIPSE) {
        shape = new ShapeEllipse(shapeParent, relPoint.x(), relPoint.y());
        shape->setName(QString("Ellipse #%1").arg(id++));

    }
    Q_ASSERT(shape != nullptr);

    // Do common stuff
    mCreatingShape = shape;
    mCreatingLastPosition = point;
    shape->collapse();
    shape->setParentItem(shapeParent);
    model->addItem(mCreatingShape, shapeParent);

    // Select the new Shape
    QModelIndex index = (QModelIndex)(*shape->parentTreeItem()->modelIndex());
    int shapeIndex = shape->parentTreeItem()->indexOf(shape);
    TreeModel::instance()->selectionsChanged(index, shapeIndex, shapeIndex);
}

void CanvasView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF point = QGraphicsView::mapToScene(event->pos());
    if (mCreatingShape) {
        QPointF delta = point - mCreatingLastPosition;
        mCreatingLastPosition = point;
        mCreatingShape->boundingBox().boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection::BOTTOM_RIGHT, delta);
    }
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    mCreatingShape = nullptr;
    mCreatingLastPosition = QPointF(0.0f, 0.0f);
}

void CanvasView::mouseDoubleClickEvent(QMouseEvent* /*event*/)
{
    if (mSelectedShape) {
        mSelectedShape->toggleEditMode();
    }
}

void CanvasView::wheelEvent(QWheelEvent *event)
{
    mZoomFactor +=  0.1f * (event->delta() / abs(event->delta()));
    mZoomFactor = qBound(0.2f, mZoomFactor, 2.0f);
    fitView();
}

void CanvasView::resizeEvent(QResizeEvent* /*event*/)
{
    fitView();
}

void CanvasView::showEvent(QShowEvent* /*event*/)
{
    fitView();
}

void CanvasView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}

void CanvasView::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat() && (
                event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
        if (mSelectedShape){
            mSelectedShape->toggleEditMode();
        }
    }
    QGraphicsView::keyReleaseEvent(event);
}

void CanvasView::onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* /*oldFocusItem*/, Qt::FocusReason /*reason*/)
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

void CanvasView::setScene(QGraphicsScene* scene)
{
    QGraphicsView::setScene(scene);
    connect(scene, &QGraphicsScene::focusItemChanged, this, &CanvasView::onFocusItemChanged);
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


