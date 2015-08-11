#include "SketchItemBezier.h"

#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

#include "BezierControlPoint.h"
#include "BezierPoint.h"

SketchItemBezier::SketchItemBezier(qreal x, qreal y)
    : SketchItem(),
      mItem(new QGraphicsPathItem),
      mPath(),
      mElements(),
      mBoundingBox(new BoundingBox(this)),
      mIsPathClosed(false),
      mIsSelected(false)
{
    mItem->setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    mItem->setBrush(QBrush(QColor(122, 163, 39)));

    mItem->setFlag(QGraphicsItem::ItemIsMovable);
    mItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    mItem->setFlag(QGraphicsItem::ItemIsSelectable);
    mItem->setFlag(QGraphicsItem::ItemIsFocusable);

    mItem->setData(SketchItem::Type::SKETCH_ITEM_BEZIER, qVariantFromValue(static_cast<void *>(this)));
    mItem->setPos(x, y);

    mBoundingBox->setVisible(false);


}

SketchItemBezier::~SketchItemBezier()
{
    delete mItem;
    for (auto p : mElements) {
        delete p;
        p = nullptr;
    }
    mElements.clear();
}

QGraphicsItem* SketchItemBezier::getGraphicsItem()
{
    return mItem;
}

void SketchItemBezier::addPath(const QPointF& c1, const QPointF& c2, const QPointF& endPos)
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

    mItem->setPath(mPath);
}

void SketchItemBezier::closePath()
{
    qDebug() << "Closing path";
    mIsPathClosed = true;
    mPath.closeSubpath();
}

void SketchItemBezier::updateElement(BezierElement* bezierElement, const QPointF& pos)
{
    int listIndex = mElements.indexOf(bezierElement);
    QPointF delta = pos - bezierElement->getPos();

    // Move control points associated to the bezier point
    if(listIndex >= 0
            && bezierElement->getElementType() == BezierElement::POINT
            && mEditMode == EditMode::BEZIER) {

        if (bezierElement == mElements.first()) {
            mElements[listIndex + 1]->moveBy(delta);

        } else if (bezierElement == mElements.last()) {
            mElements[listIndex - 1]->moveBy(delta);

        } else {
            mElements[listIndex - 1]->moveBy(delta);
            mElements[listIndex + 1]->moveBy(delta);
        }
    }

    // Moving link between the first and the last element
    if (bezierElement == mElements.last() && mIsPathClosed) {
        mElements.first()->moveBy(delta);
        mPath.setElementPositionAt(mElements.length(), pos.x(), pos.y());
    }

    // Move the current element
    mPath.setElementPositionAt(bezierElement->getIndex(), pos.x(), pos.y());
    mItem->setPath(mPath);

    // Update bounding box and handles positions
    mBoundingBox->updateRect();
}

void SketchItemBezier::boundingBoxEvent(const BoundingBoxEvent& event)
{
    for (int i = 1; i < mElements.length(); i++) {
        BezierElement* element = mElements[i];

        // Move to bounding box origin
        QPointF p1 = element->getPos() - event.origin;

        // Apply scale factor
        p1.setX(p1.x() * event.scale.x());
        p1.setY(p1.y() * event.scale.y());

        // Move back to correct referential
        p1 += event.origin;

        element->setPos(p1);
    }
}

void SketchItemBezier::setIsSelected(bool isSelected)
{
    mIsSelected = isSelected;
    qDebug() << "mIsSelected : " << mIsSelected;

    updateBoundingBoxBezierVisibility();
}

void SketchItemBezier::setEditMode(EditMode mode)
{
    mEditMode = mode;
    qDebug() << "mEditMode : " << mEditMode;

    updateBoundingBoxBezierVisibility();
}

void SketchItemBezier::updateBoundingBoxBezierVisibility()
{
    // Update bounding box visibility
    bool boundingboxVisibility = mIsSelected && mEditMode == EditMode::BOUNDING_BOX;
    qDebug() << "boundingboxVisibility : " << boundingboxVisibility;
    mBoundingBox->setVisible(boundingboxVisibility);

    // Update bezier points visibility
    bool bezierVisibility = mIsSelected && mEditMode == EditMode::BEZIER;
    qDebug() << "bezierVisibility : " << bezierVisibility;
    for (auto p : mElements) {
        p->setVisible(bezierVisibility);
    }
}

QRectF SketchItemBezier::getBounds()
{
    QRectF bounds(0.0, 0.0, 0.0, 0.0);

    if (mElements.length() <= 0) {
        return bounds;
    }

    QPointF initValue = mElements[0]->getPos();
    QPointF lower(initValue);
    QPointF higher(initValue);

    for (int i = 1; i < mElements.length(); i++) {
        BezierElement* element = mElements[i];
        QPointF position = element->getPos();

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
