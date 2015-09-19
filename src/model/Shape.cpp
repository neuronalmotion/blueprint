#include "Shape.h"

#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QPointF>

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


void Shape::appendChild(Shape* child)
{
    mChildItems.append(child);
}

bool Shape::removeChild(Shape* child)
{
    return mChildItems.removeOne(child);
}

Shape*Shape::child(int row)
{
    return mChildItems.at(row);
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

    if (shapeType() != ShapeType::CANVAS) {
        blueprint::Shape* shapeParent = dynamic_cast<blueprint::Shape*>(mParentShape);
        position = position + shapeParent->posAbsolute();
    }

    return position;
}

void Shape::setParentShape(Shape* parentShape)
{
    mParentShape = parentShape;
}
