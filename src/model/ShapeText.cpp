#include "ShapeText.h"

#include <QGraphicsTextItem>
#include <QPainter>
#include <QTextDocument>

#include "ShapeModel.h"

using namespace blueprint;

ShapeText::ShapeText(Shape* parentShape, const Shape::ShapeType& shapeType, const qreal& x, const qreal& y)
    : Shape(parentShape, shapeType),
      mGraphicsItem(new TextGraphicsItem(this)),
      mBoundingBox(this)
{
    init(x, y);
}

ShapeText::ShapeText(Shape* parentShape)
    : Shape(parentShape, Shape::ShapeType::TEXT),
      mGraphicsItem(new TextGraphicsItem(this)),
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
}

QFont ShapeText::font() const
{
    return mGraphicsItem->font();
}

void ShapeText::setFont(const QFont& font)
{
    mGraphicsItem->setFont(font);
    mGraphicsItem->adjustSize();
}

Parcel*ShapeText::toParcel() const
{
    Parcel* parcel = Shape::toParcel();
    parcel->putProperty("text", text());
    parcel->putProperty("font", font().toString());
    return parcel;
}

void ShapeText::fromParcel(const Parcel& parcel)
{
    Shape::fromParcel(parcel);
    setText(parcel.propertyValue("text").toString());
    QFont font;
    font.fromString(parcel.propertyValue("font").toString());
    setFont(font);
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

TextGraphicsItem::TextGraphicsItem(ShapeText* shape, QGraphicsItem* parent)
    : QGraphicsTextItem(parent),
      mShape(shape)
{

}

TextGraphicsItem::~TextGraphicsItem()
{

}

void TextGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setOpacity(mShape->opacity());
    QGraphicsTextItem::paint(painter, option, widget);
}

void TextGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
     QGraphicsTextItem::mouseMoveEvent(event);
     ShapeModel::instance()->shapeGeometryChanged(mShape);
}
