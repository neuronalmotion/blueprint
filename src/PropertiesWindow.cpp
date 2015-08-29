#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"

#include "model/TreeItem.h"
#include "model/TreeModel.h"
#include "model/Shape.h"

using namespace blueprint;

PropertiesWindow::PropertiesWindow(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::PropertiesWindow)
{
    mUi->setupUi(this);
    mUi->backgroundColor->setAutoFillBackground(true);
    connect(TreeModel::instance(), &TreeModel::selectionsChanged, this, &PropertiesWindow::selectionsChanged);
}

PropertiesWindow::~PropertiesWindow()
{
    delete mUi;
}

void PropertiesWindow::selectionsChanged(const QModelIndex& parent, int first, int last)
{
    TreeModel* model = TreeModel::instance();
    blueprint::Shape* item = static_cast<blueprint::Shape*>(model->itemFromParentIndex(parent, first));
    Q_ASSERT(item);

    mUi->name->setText(item->name());

    QPalette palette = mUi->backgroundColor->palette();
    palette.setColor(QPalette::Background, item->backgroundColor());
    mUi->backgroundColor->setPalette(palette);

}
