#include "ShapeText.h"

#include <QGraphicsTextItem>
#include <QTextDocument>

using namespace blueprint;

ShapeText::ShapeText(Shape* parentShape)
    : Shape(parentShape, Shape::ShapeType::TEXT),
      mGraphicsItem(new QGraphicsTextItem(parentShape->graphicsItem())),
      mBoundingBox(this)
{
    mGraphicsItem->setPlainText(name());
    mGraphicsItem->adjustSize();
    setBackgroundColor(QColor(0, 0, 0));
}

ShapeText::~ShapeText()
{

}


QGraphicsItem* ShapeText::graphicsItem() const
{
    return mGraphicsItem;
}

QRectF ShapeText::bounds() const
{
    return mGraphicsItem->boundingRect();
}

void ShapeText::resizeOnCreation(const QPointF& delta)
{
    // TODO using mGraphicsItem->setScale?
}

void ShapeText::boundingBoxEvent(const BoundingBoxEvent& event)
{

}

void ShapeText::setBackgroundColor(const QColor& color)
{
    mGraphicsItem->setDefaultTextColor(color);
}

QColor ShapeText::backgroundColor() const
{
    return mGraphicsItem->defaultTextColor();
}

void ShapeText::setBorderColor(const QColor& /* color */)
{
    // no op
}

int ShapeText::borderWidth() const
{
    return 0;
}

void ShapeText::setBorderWidth(int /* width */)
{
    // no op
}

QString ShapeText::text() const
{
    return mGraphicsItem->document()->toPlainText();
}

void ShapeText::setText(const QString& text)
{
    mGraphicsItem->setPlainText(text);
}

QFont ShapeText::font() const
{
    return mGraphicsItem->font();
}

void ShapeText::setFont(const QFont& font)
{
    mGraphicsItem->setFont(font);
}

void ShapeText::updateBoundingBoxBezierVisibility()
{

}
