#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"

#include <QColorDialog>

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

    connect(TreeModel::instance(), &TreeModel::selectionsChanged, this, &PropertiesWindow::selectionsChanged);
    connect(TreeModel::instance(), &TreeModel::propertiesChanged, this, &PropertiesWindow::selectionsChanged);
}

PropertiesWindow::~PropertiesWindow()
{
    delete mUi;
}

void PropertiesWindow::selectionsChanged(const QModelIndex& parent, int first, int last)
{
    TreeModel* model = TreeModel::instance();
    mCurrentItem = static_cast<blueprint::Shape*>(model->itemFromParentIndex(parent, first));
    Q_ASSERT(mCurrentItem);

    mUi->name->setText(mCurrentItem->name());

    QColor backColor = mCurrentItem->backgroundColor();

    mUi->backgroundColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                        .arg(backColor.red())
                                        .arg(backColor.green())
                                        .arg(backColor.blue()));


}

void PropertiesWindow::onBackgroundColorClicked()
{
    QColor color = QColorDialog::getColor(mCurrentItem->backgroundColor(), this, "Background color");
    mCurrentItem->setBackgroundColor(color);

    QModelIndex index = (QModelIndex)(*mCurrentItem->parentTreeItem()->modelIndex());
    int shapeIndex = mCurrentItem->parentTreeItem()->indexOf(mCurrentItem);
    TreeModel::instance()->propertiesChanged(index, shapeIndex, shapeIndex);
}
