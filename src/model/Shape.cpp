#include "Shape.h"

#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

#include "ShapeFactory.h"
#include "BoundingBoxPoint.h"
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
      mEditMode(EditMode::BOUNDING_BOX),
      mChildItems(),
      mModelIndex(nullptr),
      mIsSelected(false)
{
}

Shape::~Shape()
{
    delete mModelIndex;
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

void Shape::collapse()
{
    const BoundingBoxPoint* topLeft = boundingBox().boundingBoxPoint(BoundingBoxPoint::TOP_LEFT);
    const BoundingBoxPoint* bottomRight = boundingBox().boundingBoxPoint(BoundingBoxPoint::BOTTOM_RIGHT);
    QPointF delta = topLeft->pos() - bottomRight->pos();

    // Add 1 to be able to move the bounding box
    delta.setX(delta.x() + 1);
    delta.setY(delta.y() + 1);
    boundingBox().boundingBoxPointMoved(bottomRight->translationDirection(), delta);
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

    for(auto child : mChildItems) {
        parcel->addPropertyToKey("children", child->toParcel());
    }
    return parcel;
}

void Shape::fromParcel(const Parcel& parcel)
{
    mName = parcel.propertyValue("name").toString();
    mShapeType = static_cast<ShapeType>(parcel.propertyValue("type").toInt());
    QPointF pos = QPointF(parcel.propertyValue("posx").toFloat(),
                          parcel.propertyValue("posy").toFloat());
    setPos(pos);
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
