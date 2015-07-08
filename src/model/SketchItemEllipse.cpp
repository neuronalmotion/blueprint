#include "SketchItemEllipse.h"

#include <QGraphicsEllipseItem>

SketchItemEllipse::SketchItemEllipse()
    : SketchItem(),
    mItem(new QGraphicsItemGroup)
{
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(mItem);
    circle->setRect(0.0, 0.0, 20.0, 20.0);

}

SketchItemEllipse::~SketchItemEllipse()
{
    delete mItem;
}

QGraphicsItem* SketchItemEllipse::getGraphicsItem()
{
    return mItem;
}
