#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"

#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>

#include "model/Shape.h"
#include "model/ShapeBezier.h"
#include "model/ShapeText.h"
#include "model/ShapeModel.h"


using namespace blueprint;

PropertiesWindow::PropertiesWindow(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::PropertiesWindow),
    mCurrentItem(nullptr),
    mState(CLEAR)
{
    mUi->setupUi(this);
    mUi->foregroundColor->setAutoFillBackground(true);
    mUi->foregroundColor->setFocusPolicy(Qt::NoFocus);

    initSignalSlot();
}

PropertiesWindow::~PropertiesWindow()
{
    delete mUi;
}

void PropertiesWindow::initSignalSlot()
{
    // Shape (common)
    connect(mUi->foregroundColor, &QPushButton::clicked, this, &PropertiesWindow::onBackgroundColorClicked);
    connect(mUi->spinBox_positionX, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) {
        QPointF pos = mCurrentItem->pos();
        pos.setX(value);
        mCurrentItem->setPos(pos);
    });
    connect(mUi->spinBox_positionY, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) {
        QPointF pos = mCurrentItem->pos();
        pos.setY(value);
        mCurrentItem->setPos(pos);
    });
    connect(mUi->spinBox_width, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) {
        mCurrentItem->setWidth(value);
    });
    connect(mUi->spinBox_height, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) {
        mCurrentItem->setHeight(value);
    });

    // ShapeBezier
    connect(mUi->backgroundImage, &QPushButton::clicked, this, &PropertiesWindow::onBackgroundImageClicked);
    connect(mUi->borderWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PropertiesWindow::onThicknessValueChanged);

    // ShapeText
    connect(mUi->lineEditText, &QLineEdit::textChanged, this, &PropertiesWindow::onTextChanged);
    connect(mUi->buttonTextFont, &QPushButton::clicked, this, &PropertiesWindow::displayFontDialog);
    connect(mUi->buttonTextBold, &QPushButton::clicked, this, &PropertiesWindow::updateShapeTextFromProperties);
    connect(mUi->buttonTextItalic, &QPushButton::clicked, this, &PropertiesWindow::updateShapeTextFromProperties);
    connect(mUi->buttonTextUnderline, &QPushButton::clicked, this, &PropertiesWindow::updateShapeTextFromProperties);

    // ShapeModel
    connect(ShapeModel::instance(), &ShapeModel::shapeSelected, this, &PropertiesWindow::shapeSelected);
    connect(ShapeModel::instance(), &ShapeModel::shapePropertiesChanged, this, &PropertiesWindow::shapeSelected);
    connect(ShapeModel::instance(), &ShapeModel::shapeGeometryChanged, this, &PropertiesWindow::shapeGeometryChanged);
}


void PropertiesWindow::shapeSelected(blueprint::Shape* shape)
{
    if (!shape) {
        clear();
        return;
    }

    mState = SHAPE_SELECTED;
    mCurrentItem = shape;
    qDebug() << "Shape selected";

    // Name
    mUi->name->setEnabled(true);
    mUi->name->setText(shape->name());
    shapeGeometryChanged(shape);

    // Background color
    QColor backColor = shape->foregroundColor();
    mUi->foregroundColor->setEnabled(true);
    mUi->foregroundColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                        .arg(backColor.red())
                                        .arg(backColor.green())
                                        .arg(backColor.blue()));

    // Background image
    // TODO rather than relying on ugly casts
    // we should display a "modified" PropertiesWindow
    // based on the shape type / attributes
    if (shape->shapeType() != Shape::ShapeType::TEXT) {
        ShapeBezier* shapeBezier = static_cast<ShapeBezier*>(shape);
        QString backImage = shapeBezier->backgroundImageFileName();
        mUi->backgroundImageText->setText(backImage);
    } else {
        updateTextProperties();
    }

    // Border width
    mUi->borderWidth->setValue(shape->borderWidth());
}

void PropertiesWindow::shapeGeometryChanged(Shape* shape)
{
    if (shape != mCurrentItem) {
        return;
    }
    toggleSignalBlocks(true);
    mUi->spinBox_positionX->setValue(shape->posAbsolute().x());
    mUi->spinBox_positionY->setValue(shape->posAbsolute().y());
    mUi->spinBox_width->setValue(shape->width());
    mUi->spinBox_height->setValue(shape->height());
    toggleSignalBlocks(false);
}



void PropertiesWindow::onBackgroundColorClicked()
{
    QColor color = QColorDialog::getColor(mCurrentItem->foregroundColor(),
                                          this,
                                          "Background color",
                                          QColorDialog::ShowAlphaChannel);
    mCurrentItem->setForegroundColor(color);
    ShapeModel::instance()->shapePropertiesChanged(mCurrentItem);
}

void PropertiesWindow::onBackgroundImageClicked()
{
    ShapeBezier* shapeBezier = static_cast<ShapeBezier*>(mCurrentItem);
    if (!shapeBezier) {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this,
                                            "Open background image",
                                            "Image Files (*.png *.jpg *.bmp)");

    shapeBezier->setBackgroundImage(fileName);
    ShapeModel::instance()->shapePropertiesChanged(mCurrentItem);
}

void PropertiesWindow::onThicknessValueChanged(int val)
{
    mCurrentItem->setBorderWidth(val);
    ShapeModel::instance()->shapePropertiesChanged(mCurrentItem);
}

void PropertiesWindow::onTextChanged(const QString& text)
{
    // FIXME more ugly casting...
    if (mCurrentItem->shapeType() != Shape::ShapeType::TEXT) {
        return;
    }
    ShapeText* shape = static_cast<ShapeText*>(mCurrentItem);
    if (!shape) {
        return;
    }
    shape->setText(text);
}

void PropertiesWindow::displayFontDialog()
{
    if (mCurrentItem->shapeType() != Shape::ShapeType::TEXT) {
        return;
    }
    ShapeText* shape = static_cast<ShapeText*>(mCurrentItem);
    bool ok;
    QFont font = QFontDialog::getFont(&ok, shape->font(), this);
    if (ok) {
        mUi->buttonTextFont->setText(font.family());
        shape->setFont(font);
        updateShapeTextFromProperties();
    }
}

void PropertiesWindow::clear()
{
    if (mState == CLEAR) {
        return;
    }
    mState = CLEAR;
    mCurrentItem = nullptr;
    mUi->name->clear();
    mUi->lineEditText->clear();
    mUi->foregroundColor->setEnabled(false);
    mUi->backgroundImageText->clear();
    mUi->foregroundColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                        .arg(0)
                                        .arg(0)
                                        .arg(0));
    mUi->borderWidth->clear();
}

void PropertiesWindow::updateTextProperties()
{
    if (mCurrentItem->shapeType() != Shape::ShapeType::TEXT) {
        return;
    }
    ShapeText* shape = static_cast<ShapeText*>(mCurrentItem);
    QFont font = shape->font();
    mUi->lineEditText->setText(shape->text());
    mUi->buttonTextFont->setText(font.family());
    mUi->buttonTextBold->toggled(font.bold());
    mUi->buttonTextItalic->toggled(font.italic());
    mUi->buttonTextUnderline->toggled(font.underline());
}

void PropertiesWindow::updateShapeTextFromProperties()
{
    if (mCurrentItem->shapeType() != Shape::ShapeType::TEXT) {
        return;
    }
    ShapeText* shape = static_cast<ShapeText*>(mCurrentItem);
    QFont font = shape->font();
    font.setFamily(mUi->buttonTextFont->text());
    font.setBold(mUi->buttonTextBold->isChecked());
    font.setItalic(mUi->buttonTextItalic->isChecked());
    font.setUnderline(mUi->buttonTextUnderline->isChecked());
    shape->setFont(font);
}

void PropertiesWindow::toggleSignalBlocks(bool block)
{

    mUi->spinBox_positionX->blockSignals(block);
    mUi->spinBox_positionY->blockSignals(block);
    mUi->spinBox_width->blockSignals(block);
    mUi->spinBox_height->blockSignals(block);
}
