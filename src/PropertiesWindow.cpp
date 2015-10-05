#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"

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
    mCurrentItem(nullptr)
{
    mUi->setupUi(this);
    mUi->backgroundColor->setAutoFillBackground(true);
    mUi->backgroundColor->setFocusPolicy(Qt::NoFocus);

    initSignalSlot();
}

PropertiesWindow::~PropertiesWindow()
{
    delete mUi;
}

void PropertiesWindow::initSignalSlot()
{
    // Shape (common)
    connect(mUi->backgroundColor, &QPushButton::clicked, this, &PropertiesWindow::onBackgroundColorClicked);

    // ShapeBezier
    connect(mUi->backgroundImage, &QPushButton::clicked, this, &PropertiesWindow::onBackgroundImageClicked);
    connect(mUi->thickness, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PropertiesWindow::onThicknessValueChanged);

    // ShapeText
    connect(mUi->lineEditText, &QLineEdit::textChanged, this, &PropertiesWindow::onTextChanged);
    connect(mUi->buttonTextFont, &QPushButton::clicked, this, &PropertiesWindow::displayFontDialog);
    connect(mUi->buttonTextBold, &QPushButton::clicked, this, &PropertiesWindow::updateShapeTextFromProperties);
    connect(mUi->buttonTextItalic, &QPushButton::clicked, this, &PropertiesWindow::updateShapeTextFromProperties);
    connect(mUi->buttonTextUnderline, &QPushButton::clicked, this, &PropertiesWindow::updateShapeTextFromProperties);

    // ShapeModel
    connect(ShapeModel::instance(), &ShapeModel::shapeSelected, this, &PropertiesWindow::shapeSelected);
    connect(ShapeModel::instance(), &ShapeModel::shapePropertiesChanged, this, &PropertiesWindow::shapeSelected);
}


void PropertiesWindow::shapeSelected(blueprint::Shape* shape)
{
    mCurrentItem = shape;
    if (!mCurrentItem) {
        reset();
        return;
    }

    // Name
    mUi->name->setText(mCurrentItem->name());

    // Background color
    QColor backColor = mCurrentItem->backgroundColor();
    mUi->backgroundColor->setEnabled(true);
    mUi->backgroundColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                        .arg(backColor.red())
                                        .arg(backColor.green())
                                        .arg(backColor.blue()));

    // Background image
    // TODO rather than relying on ugly casts
    // we should display a "modified" PropertiesWindow
    // based on the shape type / attributes
    if (mCurrentItem->shapeType() != Shape::ShapeType::TEXT) {
        ShapeBezier* shapeBezier = static_cast<ShapeBezier*>(mCurrentItem);
        QString backImage = shapeBezier->backgroundImageFileName();
        mUi->backgroundImageText->setText(backImage);
    } else {
        updateTextProperties();
    }

    // Border width
    mUi->thickness->setValue(mCurrentItem->borderWidth());
}

void PropertiesWindow::onBackgroundColorClicked()
{
    QColor color = QColorDialog::getColor(mCurrentItem->backgroundColor(),
                                          this,
                                          "Background color",
                                          QColorDialog::ShowAlphaChannel);
    mCurrentItem->setBackgroundColor(color);
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

    if (!mCurrentItem->shapeType() == Shape::ShapeType::TEXT) {
        return;
    }
    ShapeText* shape = static_cast<ShapeText*>(mCurrentItem);
    bool ok;
    QFont font = QFontDialog::getFont(&ok, shape->font(), this);
    if (ok) {
        mUi->buttonTextFont->setText(font.family());
        updateShapeTextFromProperties();
    }
}

void PropertiesWindow::reset()
{
    mUi->name->clear();
    mUi->backgroundColor->setEnabled(false);
    mUi->backgroundImageText->clear();
    mUi->backgroundColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                        .arg(0)
                                        .arg(0)
                                        .arg(0));
    mUi->thickness->clear();
}

void PropertiesWindow::updateTextProperties()
{
    if (!mCurrentItem->shapeType() == Shape::ShapeType::TEXT) {
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
    if (!mCurrentItem->shapeType() == Shape::ShapeType::TEXT) {
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
