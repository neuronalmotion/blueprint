 #include "CanvasView.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QTransform>
#include <QtMath>

#include "Tool.h"
#include "model/BezierControlPoint.h"
#include "model/BezierPoint.h"
#include "model/Canvas.h"
#include "model/Shape.h"
#include "model/ShapeLine.h"
#include "model/ShapeModel.h"
#include "model/ShapeFactory.h"
#include "model/ShapeBezierCurve.h"

using namespace blueprint;

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent),
    mCurrentTool(Tool::Type::SELECTION),
    mCreatingShape(nullptr),
    mCreatingLastPosition(0, 0),
    mZoomFactor(1.0f)
{
    connect(ShapeModel::instance(), &ShapeModel::shapePropertiesChanged, this, &CanvasView::shapePropertiesChanged);

    setRenderHint(QPainter::Antialiasing, true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
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
    // Only selection and drag mode
    // require to call QGraphicsView implementation
    if (mCurrentTool == Tool::Type::SELECTION || dragMode() == QGraphicsView::ScrollHandDrag) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    // Search the parent item
    QPointF point = QGraphicsView::mapToScene(event->pos());
    qDebug() << "point = " << point;
    blueprint::Shape* shapeParent = shapeFromScenePoint(point);
    Q_ASSERT(shapeParent != nullptr);

    // Position is always relative to direct parent
    QPointF relPoint(point.x() - shapeParent->posAbsolute().x(),
                     point.y() - shapeParent->posAbsolute().y());

    qDebug() << "REL point = " << relPoint;

    // Create shape and set initial position
    if (mCurrentTool != Tool::BEZIER_CURVE) {
        mCreatingShape = ShapeFactory::createShape(Tool::shapeType(mCurrentTool), shapeParent);
        mCreatingShape->setPos(relPoint);

        // Snap bounding box to mouse position
        mCreatingShape->collapse();
        mCreatingLastPosition = point;
        qDebug() << "mCreatingLastPosition = " << mCreatingLastPosition;
        ShapeModel* model = ShapeModel::instance();
        model->addItem(mCreatingShape, shapeParent);
        model->selectShape(mCreatingShape);
    }else{
        if (mCreatingShape == nullptr) {
            mCreatingShape = ShapeFactory::createShape(Tool::shapeType(mCurrentTool), shapeParent);
            mCreatingShape->setPos(relPoint);

            // Snap bounding box to mouse position
            mCreatingShape->collapse();
            mCreatingLastPosition = point;
            qDebug() << "mCreatingLastPosition = " << mCreatingLastPosition;
            ShapeModel* model = ShapeModel::instance();
            model->addItem(mCreatingShape, shapeParent);
            model->selectShape(mCreatingShape);
        }else{
            ShapeBezierCurve* shapeBezierCurve = static_cast<ShapeBezierCurve*>(mCreatingShape);

            QPointF delta = point - mCreatingLastPosition;
            shapeBezierCurve->addPath(delta , delta, delta);
            //shapeBezierCurve->addSegment(delta);
            ShapeModel* model = ShapeModel::instance();
            model->selectShape(shapeBezierCurve);
            mCreatingLastPosition = point - delta;
            qDebug() << "mCreatingLastPosition = " << mCreatingLastPosition;

        }
    }

    qDebug() << " ";
    event->accept();
}

void CanvasView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    if (dragMode() == QGraphicsView::ScrollHandDrag) {
        return;
    }

    if (mCurrentTool == Tool::BEZIER_CURVE) {
        return;
    }

    QPointF point = QGraphicsView::mapToScene(event->pos());
    if (mCreatingShape) {
        QPointF delta = point - mCreatingLastPosition;
        mCreatingLastPosition = point;
        mCreatingShape->resizeOnCreation(delta);
    }

    event->accept();
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    if (dragMode() == QGraphicsView::ScrollHandDrag) {
        return;
    }

    if (mCurrentTool != Tool::BEZIER_CURVE) {
        mCreatingShape = nullptr;
        mCreatingLastPosition = QPointF(0.0f, 0.0f);
    }

    event->accept();
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
    mZoomFactor = qBound(0.2f, mZoomFactor, 5.0f);

    QMatrix matrix;
    matrix.scale(mZoomFactor, mZoomFactor);
    setMatrix(matrix);

}

void CanvasView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);

    if (event->key() == Qt::Key_Space) {
        setDragMode(ScrollHandDrag);
        setInteractive(false);
    }
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
            case Qt::Key_Space:
                setDragMode(NoDrag);
                setInteractive(true);
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
