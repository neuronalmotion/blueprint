#include "Shape.h"

#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

#include "ShapeFactory.h"

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

Shape*Shape::child(int row)
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

QPointF Shape::posAbsolute()
{
    QPointF position = graphicsItem()->pos();

    // FIXME function name is a misnomer, position is not always absolute!
    if (shapeType() != ShapeType::CANVAS) {
        blueprint::Shape* shapeParent = dynamic_cast<blueprint::Shape*>(mParentShape);
        position = position + shapeParent->posAbsolute();
    }

    return position;
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

SerializeInfo* Shape::serialize() const
{
    SerializeInfo* serializeInfo = new SerializeInfo(IO_NAME_SHAPE);
    serializeInfo->putProperty("name", mName);
    serializeInfo->putProperty("type", mShapeType);

    for(auto child : mChildItems) {
        serializeInfo->addPropertyToKey("children", child->serialize());
    }
    return serializeInfo;
}

void Shape::deserialize(const SerializeInfo& serializeInfo)
{
    mName = serializeInfo.propertyValue("name").toString();
    mShapeType = static_cast<ShapeType>(serializeInfo.propertyValue("type").toInt());
    SerializeInfo* children = serializeInfo.at("children");

    for(auto child : children->list()) {
        ShapeType childShapeType = static_cast<ShapeType>(child->propertyValue("type").toInt());
        // FIXME child coordinates should not be mandatory in Factory
        Shape* childShape = ShapeFactory::createShape(childShapeType, *this, QPointF(0, 0));
        childShape->deserialize(*child);
        insertChild(0, childShape);
    }
}
