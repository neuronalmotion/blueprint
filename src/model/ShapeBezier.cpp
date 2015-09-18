#include "ShapeBezier.h"

#include <QDebug>
#include <QtMath>

#include "BezierControlPoint.h"
#include "BezierPoint.h"

using namespace blueprint;

blueprint::ShapeBezier::ShapeBezier(TreeItem* parentTreeItem, const Shape::ShapeType shapeType, qreal x, qreal y)
    : Shape(parentTreeItem, shapeType, x, y),
      mPath(),
      mElements(),
      mGraphicsItem(new GraphicsItem(this)),
      mIsPathClosed(false),
      mBackgroundImage(nullptr),
      mBackgroundImageFileName()
{
    setBorderColor(QColor(40, 40, 40));

    // fast random color
    setBackgroundColor(QColor(qrand() % 255, qrand() % 255, qrand() % 255));

    mGraphicsItem->setFlag(QGraphicsItem::ItemIsMovable);
    mGraphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    mGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable);
    mGraphicsItem->setFlag(QGraphicsItem::ItemIsFocusable);

    mGraphicsItem->setData(0, qVariantFromValue(static_cast<void *>(this)));
    mGraphicsItem->setPos(x, y);
}

blueprint::ShapeBezier::~ShapeBezier()
{
    qDeleteAll(mElements);
}

QGraphicsItem* ShapeBezier::getQGraphicsItem()
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
}

void ShapeBezier::resizeOnCreation(const QPointF& delta)
{
    mBoundingBox.boundingBoxPointMoved(BoundingBoxPoint::TranslationDirection::BOTTOM_RIGHT, delta);
}

void ShapeBezier::setBackgroundColor(const QColor& color)
{
    mGraphicsItem->setBrush(QBrush(color));
}

QColor ShapeBezier::backgroundColor() const
{
    mGraphicsItem->brush().color();
}

void ShapeBezier::setBorderColor(const QColor& color)
{
    mGraphicsItem->setPen(QPen(color, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
}

QRectF ShapeBezier::bounds() const
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


void ShapeBezier::setBackgroundImage(const QString& fileName)
{
    mBackgroundImageFileName = fileName;

    if (mBackgroundImage){
        delete mBackgroundImage;
    }

    mBackgroundImage = new QImage(fileName);
}

void ShapeBezier::updateBoundingBoxBezierVisibility()
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

    setPath(mPath);
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
    QGraphicsPathItem::paint(painter, option, widget);

    // Draw background image
    QImage* backgroundImage = mShape->backgroundImage();
    if (backgroundImage) {
        QRectF target = boundingRect();
        QRectF source(backgroundImage->rect());
        painter->drawImage(target, *backgroundImage, source);
    }

    // Apply grey-mask on out of parent bounds for Shape
    if (mShape->itemType() == TreeItem::ItemType::SHAPE) {

        // Shape bounds
        QRectF shapeBounds = boundingRect();

        // Parent bounds (in shape coordinate!)
        blueprint::Shape* parentShape = dynamic_cast<blueprint::Shape*>(mShape->parentTreeItem());
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