#include "SketchItem.h"

#include <QDebug>

SketchItem::SketchItem()
    : mEditMode(EditMode::BOUNDING_BOX)
{

}

SketchItem::~SketchItem()
{

}

void SketchItem::setEditMode(EditMode mode)
{
    qDebug() << "SketchItem::setEditMode()";
    mEditMode = mode;
}
