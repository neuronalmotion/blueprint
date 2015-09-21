#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"

#include <QColorDialog>
#include <QFileDialog>

#include "model/Shape.h"
#include "model/ShapeBezier.h"
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

    connect(mUi->backgroundColor, &QPushButton::clicked, this, &PropertiesWindow::onBackgroundColorClicked);
    connect(mUi->backgroundImage, &QPushButton::clicked, this, &PropertiesWindow::onBackgroundImageClicked);

    connect(ShapeModel::instance(), &ShapeModel::shapeSelected, this, &PropertiesWindow::shapeSelected);
    connect(ShapeModel::instance(), &ShapeModel::shapePropertiesChanged, this, &PropertiesWindow::shapeSelected);
}

PropertiesWindow::~PropertiesWindow()
{
    delete mUi;
}

void PropertiesWindow::shapeSelected(blueprint::Shape* shape)
{
    mCurrentItem = shape;
    Q_ASSERT(mCurrentItem);

    // Name
    mUi->name->setText(mCurrentItem->name());

    // Background color
    QColor backColor = mCurrentItem->backgroundColor();
    mUi->backgroundColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                        .arg(backColor.red())
                                        .arg(backColor.green())
                                        .arg(backColor.blue()));

    // Background image
    // TODO rather than relying on ugly casts
    // we should display a "modified" PropertiesWindow
    // based on the shape type / attributes
    ShapeBezier* shapeBezier = static_cast<ShapeBezier*>(mCurrentItem);
    if (shapeBezier) {
        QString backImage = shapeBezier->backgroundImageFileName();
        mUi->backgroundImageText->setText(backImage);
    }
}

void PropertiesWindow::onBackgroundColorClicked()
{
    QColor color = QColorDialog::getColor(mCurrentItem->backgroundColor(), this, "Background color", QColorDialog::ShowAlphaChannel);
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
