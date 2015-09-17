#include "Shape.h"

#include <QtMath>
#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QImage>

#include "BezierControlPoint.h"
#include "BezierPoint.h"

using namespace blueprint;

Shape* Shape::fromQGraphicsItem(const QGraphicsItem& item)
{
    QVariant itemVariant = item.data(0);
    return static_cast<Shape*>(itemVariant.value<void *>());
}

Shape::Shape(TreeItem* parentItem, const ShapeType shapeType, qreal x, qreal y)
    : TreeItem(ItemType::SHAPE, parentItem),
      QGraphicsPathItem(),
      mPath(),
      mShapeType(shapeType),
      mElements(),
      mBoundingBox(this),
      mIsPathClosed(false),
      mEditMode(EditMode::BOUNDING_BOX),
      mBackgroundImage(nullptr)
{
    setBorderColor(QColor(40, 40, 40));

    // fast random color
    setBackgroundColor(QColor(qrand() % 255, qrand() % 255, qrand() % 255));

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);

    setData(0, qVariantFromValue(static_cast<void *>(this)));
    setPos(x, y);

    mBoundingBox.setVisible(false);
}

Shape::~Shape()
{
    qDeleteAll(mElements);
}

void Shape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // Draw original shape
    QGraphicsPathItem::paint(painter, option, widget);

    // Draw background image
    if (mBackgroundImage) {
        QRectF target = boundingRect();
        QRectF source(mBackgroundImage->rect());
        painter->drawImage(target, *mBackgroundImage, source);
    }

    // Apply grey-mask on out of parent bounds for Shape
    if (itemType() == TreeItem::ItemType::SHAPE) {

        // Shape bounds
        QRectF shapeBounds = boundingRect();

        // Parent bounds (in shape coordinate!)
        blueprint::Shape* parentShape = dynamic_cast<blueprint::Shape*>(mParentItem);
        QRectF parentBounds = parentShape->boundingRect();
        parentBounds.moveTo(-pos().x() - shapeBounds.x(), -pos().y() - shapeBounds.y());

        // Temp buffer
        QImage buffer = QImage(shapeBounds.width(), shapeBounds.height(), QImage::Format_ARGB32_Premultiplied);
        buffer.fill(0);
        QPainter p(&buffer);
        p.setRenderHint(QPainter::Antialiasing);

        //Draw base (parent bounds)
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 255));
        p.drawRect(parentBounds);

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

void Shape::addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos)
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

    setPath(mPath);
}

void Shape::closePath()
{
    qDebug() << "Closing path";
    mIsPathClosed = true;
    mPath.closeSubpath();
}

void Shape::updateElement(BezierElement* bezierElement, const QPointF& pos)
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
            && mEditMode == EditMode::BEZIER) {

        if (mShapeType == ShapeType::LINE) {

            // Get Slope
            QPointF refPos = mElements.first()->pos();
            QPointF firstElementPos = mElements.first()->pos();
            QPointF lastElementPos = mElements.last()->pos();
            firstElementPos -= refPos;
            lastElementPos -= refPos;

            float distance = qSqrt(qPow(lastElementPos.x(), 2) + qPow(lastElementPos.y(), 2));
            QPointF slope = lastElementPos / distance;

            // Update new control first point position
            float firstControlPointDistance = distance * 0.1f;
            mElements[1]->setPos((slope * firstControlPointDistance) + refPos);

            // Update new control last point position
            float lastControlPointDistance = distance * 0.9f;
            mElements[mElements.length() - 2]->setPos((slope * lastControlPointDistance) + refPos);

        } else {

            if (bezierElement == mElements.first()) {
                mElements[listIndex + 1]->moveBy(delta);

            } else if (bezierElement == mElements.last()) {
                mElements[listIndex - 1]->moveBy(delta);

            } else {
                mElements[listIndex - 1]->moveBy(delta);
                mElements[listIndex + 1]->moveBy(delta);
            }
        }
    }


    // Update path
    setPath(mPath);

    // Update bounding box and handles positions
    mBoundingBox.updateRect();
}

void Shape::boundingBoxEvent(const BoundingBoxEvent& event)
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
}

void Shape::resizeOnCreation(const QPointF& delta)
{
     mBoundingBox.boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection::BOTTOM_RIGHT, delta);
}

void Shape::setSelected(bool selected)
{
    QGraphicsPathItem::setSelected(selected);

    TreeItem::setSelected(selected);
    qDebug() << "mIsSelected : " << mIsSelected;

    updateBoundingBoxBezierVisibility();
}

void Shape::toggleEditMode()
{
    EditMode nextEditMode = editMode() == EditMode::BEZIER ?
                EditMode::BOUNDING_BOX
              : EditMode::BEZIER;
    setEditMode(nextEditMode);
}

void Shape::setEditMode(const EditMode& mode)
{
    mEditMode = mode;
    qDebug() << "mEditMode : " << mEditMode;

    updateBoundingBoxBezierVisibility();
}

void Shape::updateBoundingBoxBezierVisibility()
{
    // Update bounding box visibility
    bool boundingboxVisibility = mIsSelected && mEditMode == EditMode::BOUNDING_BOX;
    qDebug() << "boundingboxVisibility : " << boundingboxVisibility;
    mBoundingBox.setVisible(boundingboxVisibility);

    // Update bezier points visibility
    bool bezierVisibility = mIsSelected && mEditMode == EditMode::BEZIER;
    qDebug() << "bezierVisibility : " << bezierVisibility;
    for (auto p : mElements) {
        p->setVisible(bezierVisibility);
    }
}

QRectF Shape::bounds() const
{
    QRectF bounds(0.0, 0.0, 0.0, 0.0);

    if (mElements.length() <= 0) {
        return bounds;
    }

    QPointF initValue = mElements[0]->pos();
    QPointF lower(initValue);
    QPointF higher(initValue);

    for (int i = 1; i < mElements.length(); i++) {
        BezierElement* element = mElements[i];
        QPointF position = element->pos();

        if (position.x() < lower.x()) {
            lower.setX(position.x());
        }

        if (position.y() < lower.y()) {
            lower.setY(position.y());
        }

        if (position.x() > higher.x()) {
            higher.setX(position.x());
        }

        if (position.y() > higher.y()) {
            higher.setY(position.y());
        }
    }

    bounds.setX(lower.x());
    bounds.setY(lower.y());
    bounds.setWidth(higher.x() - lower.x());
    bounds.setHeight(higher.y() - lower.y());

    return bounds;
}

void Shape::setBackgroundColor(const QColor& color)
{
    setBrush(QBrush(color));
}

void Shape::setBorderColor(const QColor& color)
{
    setPen(QPen(color, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
}

void Shape::setBackgroundImage(const QString& fileName)
{
    mBackgroundImageFileName = fileName;

    if (mBackgroundImage){
        delete mBackgroundImage;
    }

    mBackgroundImage = new QImage(fileName);
}

QPointF Shape::posAbsolute()
{
    QPointF position = pos();

    if (itemType() != TreeItem::ItemType::CANVAS) {
        blueprint::Shape* shapeParent = dynamic_cast<blueprint::Shape*>(mParentItem);
        position = position + shapeParent->posAbsolute();
    }

    return position;
}
