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

Shape::Shape(TreeItem* parentItem, const ShapeType shapeType, qreal x, qreal y)
    : TreeItem(ItemType::SHAPE, parentItem),
      mShapeType(shapeType),
      mEditMode(EditMode::BOUNDING_BOX)
{
}

Shape::~Shape()
{
}

void Shape::setSelected(bool selected)
{
    graphicsItem()->setSelected(selected);

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

QPointF Shape::posAbsolute()
{
    QPointF position = graphicsItem()->pos();

    if (itemType() != TreeItem::ItemType::CANVAS) {
        blueprint::Shape* shapeParent = dynamic_cast<blueprint::Shape*>(mParentItem);
        position = position + shapeParent->posAbsolute();
    }

    return position;
}
