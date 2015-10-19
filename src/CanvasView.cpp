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
#include "model/ShapeLine.h"
#include "model/ShapeRectangle.h"
#include "model/ShapeEllipse.h"
#include "model/ShapeModel.h"
#include "model/ShapeFactory.h"

using namespace blueprint;

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent),
    mCurrentTool(Tool::Type::SELECTION),
    mCreatingShape(nullptr),
    mCreatingLastPosition(0, 0),
    mZoomFactor(1.0f)
{
    connect(ShapeModel::instance(), &ShapeModel::shapePropertiesChanged, this, &CanvasView::shapePropertiesChanged);
}

CanvasView::~CanvasView()
{
}

void CanvasView::shapePropertiesChanged(blueprint::Shape* shape)
{
    Q_ASSERT(shape);
    shape->graphicsItem()->update();
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

    // Search the parent item
    QPointF point = QGraphicsView::mapToScene(event->pos());
    blueprint::Shape* shapeParent = shapeFromScenePoint(point);

    // Position is always relative to direct parent
    QPointF relPoint(point.x() - shapeParent->posAbsolute().x(),
                     point.y() - shapeParent->posAbsolute().y());

    mCreatingShape = ShapeFactory::createShape(Tool::shapeType(mCurrentTool),
                                               shapeParent);

    mCreatingShape->setPos(relPoint);

    // snap bounding box to mouse position
    mCreatingShape->collapse();
    mCreatingLastPosition = point;
    ShapeModel* model = ShapeModel::instance();
    model->addItem(mCreatingShape, shapeParent);
    model->selectShape(mCreatingShape);
}

void CanvasView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF point = QGraphicsView::mapToScene(event->pos());
    if (mCreatingShape) {
        QPointF delta = point - mCreatingLastPosition;
        mCreatingLastPosition = point;
        mCreatingShape->resizeOnCreation(delta);
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
    ShapeModel* model = ShapeModel::instance();
    if (model->selectedShape()) {
        model->selectedShape()->toggleEditMode();
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
    ShapeModel* model = ShapeModel::instance();
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (model->selectedShape()){
                model->selectedShape()->toggleEditMode();
            }
        break;

        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            if (model->selectedShape()){
                model->removeItem(model->selectedShape());
            }
        break;
        default:
        break;
        }
    }
    QGraphicsView::keyReleaseEvent(event);
}

blueprint::Shape*CanvasView::shapeFromScenePoint(const QPointF& point)
{
    QGraphicsItem* graphicsItem = scene()->itemAt(point, QTransform());
    if (graphicsItem == nullptr) {
          return nullptr;
    }
    // Position is always relative to direct parent
    return blueprint::Shape::fromQGraphicsItem(*graphicsItem);
}

void CanvasView::onFocusItemChanged(QGraphicsItem* newFocusItem, QGraphicsItem* /*oldFocusItem*/, Qt::FocusReason /*reason*/)
{
    blueprint::Shape* shape = blueprint::Shape::fromQGraphicsItem(*newFocusItem);
    if (!shape) {
        return;
    }
    ShapeModel::instance()->selectShape(shape);
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


