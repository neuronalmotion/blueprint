#include "ShapeText.h"

#include <QGraphicsTextItem>
#include <QTextDocument>

using namespace blueprint;

ShapeText::ShapeText(Shape* parentShape, const Shape::ShapeType& shapeType, const qreal& x, const qreal& y)
    : Shape(parentShape, shapeType),
      mGraphicsItem(new QGraphicsTextItem(parentShape->graphicsItem())),
      mBoundingBox(this)
{
    init(x, y);
}

ShapeText::ShapeText(Shape* parentShape)
    : Shape(parentShape, Shape::ShapeType::TEXT),
      mGraphicsItem(new QGraphicsTextItem(parentShape->graphicsItem())),
      mBoundingBox(this)
{
    init(0, 0);
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
    // no op, text size should be changed by font size
}

void ShapeText::setForegroundColor(const QColor& color)
{
    mGraphicsItem->setDefaultTextColor(color);
}

QColor ShapeText::foregroundColor() const
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
    mGraphicsItem->adjustSize();
    mBoundingBox.updateRect();
}

QFont ShapeText::font() const
{
    return mGraphicsItem->font();
}

void ShapeText::setFont(const QFont& font)
{
    mGraphicsItem->setFont(font);
    mGraphicsItem->adjustSize();
    mBoundingBox.updateRect();
}

void ShapeText::updateBoundingBoxBezierVisibility()
{
    // no op, text only display Qt bounding box
}

void ShapeText::init(qreal x, qreal y)
{
    mBoundingBox.setVisible(false);
    setForegroundColor(QColor(0, 0, 0));

    mGraphicsItem->setPlainText(name());
    mGraphicsItem->adjustSize();
    mGraphicsItem->setFlag(QGraphicsItem::ItemIsMovable);
    mGraphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    mGraphicsItem->setFlag(QGraphicsItem::ItemIsSelectable);
    mGraphicsItem->setFlag(QGraphicsItem::ItemIsFocusable);


    mGraphicsItem->setData(0, qVariantFromValue(static_cast<void *>(this)));
    mGraphicsItem->setPos(x, y);
}
