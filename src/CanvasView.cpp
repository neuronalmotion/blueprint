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

using namespace blueprint;

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent),
    mCurrentTool(Tool::Type::SELECTION),
    mSelectedShape(nullptr),
    mCreatingShape(nullptr),
    mCreatingLastPosition(0, 0),
    mZoomFactor(1.0f)
{
    connect(ShapeModel::instance(), &ShapeModel::shapeSelected, this, &CanvasView::shapeSelected);
    connect(ShapeModel::instance(), &ShapeModel::shapePropertiesChanged, this, &CanvasView::shapePropertiesChanged);
}

CanvasView::~CanvasView()
{
}

void CanvasView::shapeSelected(blueprint::Shape* shape)
{
    if (shape == mSelectedShape) {
       return;
    }
    if (mSelectedShape) {
       mSelectedShape->setSelected(false);
    }

    mSelectedShape = shape;
    if (mSelectedShape) {
        shape->setSelected(true);
        qDebug() << "Selected item " << mSelectedShape->name();
    }
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

    // ::: Shape Creation tool actions :::
    blueprint::Shape* shape = nullptr;

    // Search the parent item
    blueprint::Shape* shapeParent = nullptr;
    QPointF point = QGraphicsView::mapToScene(event->pos());
    ShapeModel* model = ShapeModel::instance();
    static uint id = 0;

    QGraphicsItem* graphicsItem = scene()->itemAt(point, QTransform());

    // Can't add a shape outside of another Shape (or a Canvas)
    if (graphicsItem == nullptr) {
          return;
    }

    // Position is always relative to direct parent
    shapeParent = blueprint::Shape::fromQGraphicsItem(*graphicsItem);
    QPointF relPoint(point.x() - shapeParent->posAbsolute().x(),  point.y() - shapeParent->posAbsolute().y());

    // Create the right Shape
    QString name;
    switch (mCurrentTool) {
    case Tool::Type::RECTANGLE:
        shape = new ShapeRectangle(shapeParent, relPoint.x(), relPoint.y());
        name = "Rectangle";
        break;

    case Tool::Type::ELLIPSE:
        shape = new ShapeEllipse(shapeParent, relPoint.x(), relPoint.y());
        name = "Ellipse";
        break;

    case Tool::Type::LINE:
        shape = new ShapeLine(shapeParent, relPoint.x(), relPoint.y());
        name = "Line";
        break;
    default:
        break;
    }
    Q_ASSERT(shape != nullptr);
    shape->setName(QString("%1 %2").arg(name).arg(id++));

    // Do common stuff
    mCreatingShape = shape;
    mCreatingLastPosition = point;
    shape->collapse();
    shape->graphicsItem()->setParentItem(shapeParent->graphicsItem());
    model->addItem(mCreatingShape, shapeParent);

    // Select the new Shape
    ShapeModel::instance()->selectShape(shape);
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
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (mSelectedShape){
                mSelectedShape->toggleEditMode();
            }
        break;

        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            if (mSelectedShape){
                ShapeModel* model = ShapeModel::instance();
                model->removeItem(mSelectedShape);
            }
        break;
        default:
        break;
        }
    }
    QGraphicsView::keyReleaseEvent(event);
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


