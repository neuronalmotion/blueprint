#include "Shape.h"

#include <QDebug>
#include <QPen>
#include <QPointF>

#include "BoundingBoxPoint.h"
#include "ShapeFactory.h"
#include "ShapeModel.h"

using namespace blueprint;

Shape* Shape::fromQGraphicsItem(const QGraphicsItem& item)
{
    QVariant itemVariant = item.data(0);
    return static_cast<Shape*>(itemVariant.value<void *>());
}

static int id = 1;
Shape::Shape(Shape* parentShape, const ShapeType& shapeType)
    :
      mParentShape(parentShape),
      mShapeType(shapeType),
      mName(QString("GraphicalItem #%1").arg(id++)),
      mBoundingBox(new BoundingBox(this)),
      mEditMode(EditMode::BOUNDING_BOX),
      mChildItems(),
      mModelIndex(nullptr),
      mIsSelected(false),
      mOpacity(1.0f)
{
}

Shape::~Shape()
{
    delete mModelIndex;
    delete mBoundingBox;
    qDeleteAll(mChildItems);
}


void Shape::insertChild(int index, Shape* child)
{
    mChildItems.insert(index, child);
    child->setParentShape(this);
}

bool Shape::removeChild(Shape* child)
{
    return mChildItems.removeOne(child);
}

void Shape::removeChildAt(int index)
{
    mChildItems.removeAt(index);
}

Shape*Shape::child(int row) const
{
    return mChildItems.at(row);
}

void Shape::appendChild(Shape* child)
{
    mChildItems.append(child);
}

int Shape::indexOf(const Shape* child) const
{
    for (int i = 0; i < mChildItems.length(); ++i) {
        if (mChildItems[i] == child) {
            return i;
        }
    }
    return -1;
}

int Shape::childCount() const
{
    return mChildItems.length();
}

int Shape::columnCount() const
{
    return 1;
}

int Shape::row() const
{
    if (mParentShape) {
        return mParentShape->mChildItems.indexOf((const_cast<Shape*>(this)));
    }
    return 0;
}

void Shape::setSelected(bool selected)
{
    graphicsItem()->setSelected(selected);
    mIsSelected = selected;
    qDebug() << "mIsSelected : " << mIsSelected;
    updateBoundingBoxBezierVisibility();
}

void Shape::toggleEditMode()
{
    EditMode nextEditMode = editMode() == EditMode::PATH ?
                EditMode::BOUNDING_BOX
              : EditMode::PATH;
    setEditMode(nextEditMode);
}

void Shape::setEditMode(const EditMode& mode)
{
    mEditMode = mode;
    qDebug() << "mEditMode : " << mEditMode;

    updateBoundingBoxBezierVisibility();
}

QPointF Shape::posAbsolute()
{
    QPointF position = graphicsItem()->pos();

    // FIXME function name is a misnomer, position is not always absolute!
    if (shapeType() != ShapeType::CANVAS && mParentShape) {
        blueprint::Shape* shapeParent = dynamic_cast<blueprint::Shape*>(mParentShape);
        position = position + shapeParent->posAbsolute();
    }

    return position;
}

QPointF Shape::pos() const
{
    return graphicsItem()->pos();
}

void Shape::setPos(const QPointF& pos)
{
    graphicsItem()->setPos(pos);
}

void Shape::setWidth(const qreal& width)
{
    QPointF delta(width - this->width(), 0);
    mBoundingBox->boundingBoxPointMoved(BoundingBoxPoint::TOP_RIGHT, delta);
}

qreal Shape::width() const
{
    return bounds().width();
}

qreal Shape::height() const
{
    return bounds().height();
}

void Shape::setHeight(const qreal& height)
{
    QPointF delta(0, height - this->height());
    mBoundingBox->boundingBoxPointMoved(BoundingBoxPoint::BOTTOM_RIGHT, delta);
}

void Shape::collapse()
{
    const BoundingBoxPoint* topLeft = mBoundingBox->boundingBoxPoint(BoundingBoxPoint::TOP_LEFT);
    const BoundingBoxPoint* bottomRight = mBoundingBox->boundingBoxPoint(BoundingBoxPoint::BOTTOM_RIGHT);
    QPointF delta = topLeft->pos() - bottomRight->pos();

    // Add 1 to be able to move the bounding box
    delta.setX(delta.x() + 1);
    delta.setY(delta.y() + 1);
    mBoundingBox->boundingBoxPointMoved(bottomRight->translationDirection(), delta);
}

void Shape::setOpacity(qreal opacity)
{
    mOpacity = opacity;
    graphicsItem()->update();
}

qreal Shape::zValue()
{
    return graphicsItem()->zValue();
}

void Shape::setZValue(qreal zValue)
{
    graphicsItem()->setZValue(zValue);
}

void Shape::setParentShape(Shape* parentShape)
{
    mParentShape = parentShape;
}

Parcel* Shape::toParcel() const
{
    Parcel* parcel = new Parcel("shape");
    parcel->putProperty("name", mName);
    parcel->putProperty("type", mShapeType);
    parcel->putProperty("posx", pos().x());
    parcel->putProperty("posy", pos().y());
    parcel->putProperty("foregroundColor", foregroundColor());

    for(auto child : mChildItems) {
        parcel->addPropertyToKey("children", child->toParcel());
    }
    return parcel;
}

void Shape::fromParcel(const Parcel& parcel)
{
    mName = parcel.propertyValue("name").toString();
    mShapeType = static_cast<ShapeType>(parcel.propertyValue("type").toInt());
    setPos(QPointF(parcel.propertyValue("posx").toFloat(),
                          parcel.propertyValue("posy").toFloat()));
    setForegroundColor(parcel.propertyValue("foregroundColor").value<QColor>());
    if (parcel.contains("children")) {
        Parcel* children = parcel.at("children");

        for(auto child : children->list()) {
            ShapeType childShapeType = static_cast<ShapeType>(child->propertyValue("type").toInt());
            Shape* childShape = ShapeFactory::createShape(childShapeType, this, false);
            childShape->fromParcel(*child);
            ShapeModel::instance()->addItem(childShape, this, ShapeModel::PARCEL);
        }
    }
}
