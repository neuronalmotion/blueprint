#include "ShapeBezier.h"

#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QPainter>

#include "BezierControlPoint.h"
#include "BezierPoint.h"
#include "ShapeModel.h"

using namespace blueprint;

blueprint::ShapeBezier::ShapeBezier(Shape* parentShape,
                                    const ShapeType& shapeType,
                                    const qreal& x, const qreal& y)
    : Shape(parentShape, shapeType),
      mPath(),
      mElements(),
      mIsPathClosed(false),
      mGraphicsItem(new GraphicsItem(this)),
      mBackgroundImage(nullptr),
      mBackgroundImageFileName()
{
    init(x, y);
}

ShapeBezier::ShapeBezier(Shape* parentShape)
    : Shape(parentShape, ShapeType::BEZIER),
      mPath(),
      mElements(),
      mIsPathClosed(false),
      mGraphicsItem(new GraphicsItem(this)),
      mBackgroundImage(nullptr),
      mBackgroundImageFileName()
{
    init(0, 0);
}

void ShapeBezier::init(qreal x, qreal y)
{
    mBoundingBox->setVisible(false);
    setBorderColor(QColor(40, 40, 40));

    // fast random color
    setForegroundColor(QColor(qrand() % 255, qrand() % 255, qrand() % 255));

    mGraphicsItem->setFlag(QGraphicsItem::ItemIsMovable);
    mGraphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    mGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable);
    mGraphicsItem->setFlag(QGraphicsItem::ItemIsFocusable);

    mGraphicsItem->setData(0, qVariantFromValue(static_cast<void *>(this)));
    mGraphicsItem->setPos(x, y);
    mBoundingBox->updateParentGraphicsItem();
}

blueprint::ShapeBezier::~ShapeBezier()
{
    qDeleteAll(mElements);
}

QGraphicsItem* ShapeBezier::graphicsItem() const
{
    return mGraphicsItem;
}

void ShapeBezier::boundingBoxEvent(const BoundingBoxEvent& event)
{
    for (int i = 1; i < mElements.length(); i++) {
        BezierElement* element = mElements[i];

        // Move to bounding box origin
        QPointF p1 = element->pos() - event.origin;

        // Apply scale factor
        p1.setX(p1.x() * event.scale.x());
        p1.setY(p1.y() * event.scale.y());

        // Move back to correct referential
        p1 += event.origin;

        element->setPos(p1);
    }
    ShapeModel::instance()->shapeGeometryChanged(this);
}

void ShapeBezier::resizeOnCreation(const QPointF& delta)
{
    mBoundingBox->boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection::BOTTOM_RIGHT, delta);
}

void ShapeBezier::setForegroundColor(const QColor& color)
{
    mGraphicsItem->setBrush(QBrush(color));
}

QColor ShapeBezier::foregroundColor() const
{
    return mGraphicsItem->brush().color();
}

void ShapeBezier::setBorderColor(const QColor& color)
{
    QPen pen = mGraphicsItem->pen();
    pen.setColor(color);
    mGraphicsItem->setPen(pen);
}

int ShapeBezier::borderWidth() const
{
    return mGraphicsItem->pen().width();
}

void ShapeBezier::setBorderWidth(int width)
{
    QPen pen = mGraphicsItem->pen();
    pen.setWidth(width);
    mGraphicsItem->setPen(pen);
}

QRectF ShapeBezier::bounds() const
{
    return mPath.controlPointRect();
}

void ShapeBezier::setBackgroundImage(const QString& fileName)
{
    mBackgroundImageFileName = fileName;
    if (mBackgroundImage){
        delete mBackgroundImage;
    }
    mBackgroundImage = new QImage(fileName);
}

Parcel* ShapeBezier::toParcel() const
{
    Parcel* parcel = Shape::toParcel();
    parcel->putProperty("closed", mIsPathClosed);
    for (auto element : mElements) {
        parcel->addPropertyToKey("elements", element->toParcel());
    }
    return parcel;
}

void ShapeBezier::fromParcel(const Parcel& parcel)
{
    Shape::fromParcel(parcel);
    mIsPathClosed = parcel.propertyValue("closed").toBool();
    bezierElementsFromParcel(parcel);
}

void ShapeBezier::bezierElementsFromParcel(const Parcel& parcel)
{
    if (!parcel.contains("elements")) {
        return;
    }
    const QList<Parcel*> elements = parcel.at("elements")->list();
    int elementsSize = elements.size();
    const int pathLength = 3;
    BezierElement* path[pathLength] = {};
    for (int i = 1; i < elementsSize; i += pathLength) {
        for(int j = 0; j < pathLength; ++j) {
            path[j] =  BezierElement::bezierElementFromParcel(*elements[i + j], this);
        }
        addPath(path[0]->pos(), path[1]->pos(), path[2]->pos());
        for(int j = 0; j < pathLength; ++j) {
            delete path[j];
        }
    }
    if (mIsPathClosed) {
        closePath();
    }
}

void ShapeBezier::updateBoundingBoxBezierVisibility()
{
    // Update bounding box visibility
    bool boundingboxVisibility = mIsSelected && mEditMode == EditMode::BOUNDING_BOX;
    qDebug() << "boundingboxVisibility : " << boundingboxVisibility;
    mBoundingBox->setVisible(boundingboxVisibility);

    // Update bezier points visibility
    bool bezierVisibility = mIsSelected && mEditMode == EditMode::PATH;
    qDebug() << "bezierVisibility : " << bezierVisibility;
    for (auto p : mElements) {
        p->setVisible(bezierVisibility);
    }
}

void ShapeBezier::addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos)
{
    qDebug() << "Adding path: " << mPath.currentPosition()
             << "=>" << endPos
             << "[c1" << c1
             << "c2" << c2 << "]";

    int startElementIndex = mPath.elementCount() <= 0 ? 0 : mPath.elementCount();
    mPath.cubicTo(c1, c2, endPos);

    // create bezier elements
    int elementCount = mPath.elementCount();
    for (int i = startElementIndex; i < elementCount; ++i) {
         QPainterPath::Element element = mPath.elementAt(i);
         qDebug() << "Element:" << element << element.type;

         BezierElement* bezierElement = nullptr;
         // start point only if we are starting a path
         if ((startElementIndex == 0 && i == 0)
                 || i == elementCount - 1) {
             bezierElement = new BezierPoint(this, i);
         } else {
             bezierElement = new BezierControlPoint(this, i);
         }
         bezierElement->setPos(QPointF(element.x, element.y));
         mElements.append(bezierElement);
    }

    mGraphicsItem->setPath(mPath);
    updateBoundingBoxBezierVisibility();
}

void ShapeBezier::addSegment(const QPointF& point)
{
    QPointF lastPointPos(0 ,0);
    if (!mElements.empty()) {
        lastPointPos = mElements.last()->pos();
    }
    addPath(lastPointPos, point, point);
}

void ShapeBezier::closePath()
{
    qDebug() << "Closing path";
    mIsPathClosed = true;
    mPath.closeSubpath();
}

void ShapeBezier::updateElement(BezierElement* bezierElement, const QPointF& pos)
{
    int listIndex = mElements.indexOf(bezierElement);
    QPointF delta = pos - bezierElement->pos();

    // Moving link between the first and the last element
    if (bezierElement == mElements.last() && mIsPathClosed) {
        mElements.first()->moveBy(delta);
        mPath.setElementPositionAt(mElements.length(), pos.x(), pos.y());
    }

    // Move the current element
    mPath.setElementPositionAt(bezierElement->index(), pos.x(), pos.y());

    // Move control points associated to the bezier point
    if (listIndex >= 0
            && bezierElement->elementType() == BezierElement::POINT
            && mEditMode == EditMode::PATH) {
        if (bezierElement == mElements.first()) {
            mElements[listIndex + 1]->moveBy(delta);

        } else if (bezierElement == mElements.last()) {
            mElements[listIndex - 1]->moveBy(delta);

        } else {
            mElements[listIndex - 1]->moveBy(delta);
            mElements[listIndex + 1]->moveBy(delta);
        }
    }

    // Update path
    mGraphicsItem->setPath(mPath);

    // Update bounding box and handles positions
    mBoundingBox->updateRect();
}

// ----------------------------------------------------------------------------
// GraphicsItem
// ----------------------------------------------------------------------------

GraphicsItem::GraphicsItem(ShapeBezier* shape, QGraphicsItem* parent)
    : QGraphicsPathItem(parent),
    mShape(shape)
{

}

GraphicsItem::~GraphicsItem()
{

}

void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // Draw original shape
    painter->setOpacity(mShape->opacity());
    QGraphicsPathItem::paint(painter, option, widget);

    // Draw background image
    QImage* backgroundImage = mShape->backgroundImage();
    if (backgroundImage) {
        QRectF target = boundingRect();
        QRectF source(backgroundImage->rect());
        painter->drawImage(target, *backgroundImage, source);

        // Shape bounds
        QRectF shapeBounds = boundingRect();

        // Parent bounds (in shape coordinate!)
        blueprint::Shape* parentShape = mShape->parentShape();
        QRectF parentBounds;
        if (parentShape) {
            parentBounds = parentShape->graphicsItem()->boundingRect();
            parentBounds.moveTo(-pos().x() - shapeBounds.x(), -pos().y() - shapeBounds.y());
        }

        // Temp buffer
        QImage buffer = QImage(shapeBounds.width(), shapeBounds.height(), QImage::Format_ARGB32_Premultiplied);
        buffer.fill(0);
        QPainter p(&buffer);
        p.setRenderHint(QPainter::Antialiasing);

        //Draw base (parent bounds)
        if (parentShape) {
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(0, 0, 0, 255));
            p.drawRect(parentBounds);
        }

        // Apply composition
        p.setCompositionMode(QPainter::CompositionMode_SourceOut);

        //Draw source (grey mask)
        QRectF drawTargetRect(0, 0, buffer.width(), buffer.height());
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(200,200,200,235));
        p.drawRect(drawTargetRect);

        // Draw final image
        painter->drawImage(shapeBounds, buffer);
    }
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
     QGraphicsItem::mouseMoveEvent(event);
     ShapeModel::instance()->shapeGeometryChanged(mShape);
}
