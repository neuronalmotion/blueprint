#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"

#include <QColorDialog>
#include <QFileDialog>

#include "model/TreeItem.h"
#include "model/TreeModel.h"


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

    connect(TreeModel::instance(), &TreeModel::selectionsChanged, this, &PropertiesWindow::selectionsChanged);
    connect(TreeModel::instance(), &TreeModel::propertiesChanged, this, &PropertiesWindow::selectionsChanged);
}

PropertiesWindow::~PropertiesWindow()
{
    delete mUi;
}

void PropertiesWindow::selectionsChanged(const QModelIndex& parent, int first, int /*last*/)
{
    TreeModel* model = TreeModel::instance();
    mCurrentItem = static_cast<blueprint::Shape*>(model->itemFromParentIndex(parent, first));
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
    QString backImage = mCurrentItem->backgroundImageFileName();
    mUi->backgroundImageText->setText(backImage);

}

void PropertiesWindow::onBackgroundColorClicked()
{
    QColor color = QColorDialog::getColor(mCurrentItem->backgroundColor(), this, "Background color", QColorDialog::ShowAlphaChannel);
    mCurrentItem->setBackgroundColor(color);

    QModelIndex index = (QModelIndex)(*mCurrentItem->parentTreeItem()->modelIndex());
    int shapeIndex = mCurrentItem->parentTreeItem()->indexOf(mCurrentItem);
    TreeModel::instance()->propertiesChanged(index, shapeIndex, shapeIndex);
}

void PropertiesWindow::onBackgroundImageClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            "Open background image",
                                            "Image Files (*.png *.jpg *.bmp)");

    mCurrentItem->setBackgroundImage(fileName);

    QModelIndex index = (QModelIndex)(*mCurrentItem->parentTreeItem()->modelIndex());
    int shapeIndex = mCurrentItem->parentTreeItem()->indexOf(mCurrentItem);
    TreeModel::instance()->propertiesChanged(index, shapeIndex, shapeIndex);
}
