#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#include "CanvasView.h"
#include "model/Blueprint.h"
#include "model/Page.h"
#include "model/Canvas.h"
#include "model/Shape.h"
#include "model/ShapeRectangle.h"
#include "model/ShapeEllipse.h"
#include "model/BezierPoint.h"
#include "model/BezierControlPoint.h"
#include "model/TreeItem.h"
#include "model/TreeModel.h"

using namespace blueprint;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentBlueprint(nullptr)
{
    mUi->setupUi(this);
    resize(1200,700);

    initToolbar();

    mCurrentBlueprint = new Blueprint();
    mCurrentBlueprint->setName("Blueprint");

    // Page 1
    Page* p1 = new Page(mCurrentBlueprint);
    p1->setName("Page 1");

    // Canvas 1
    Canvas* c1 = new Canvas(p1, 0, 0);
    c1->setName("Canvas 1");
    mUi->canvas->scene()->addItem(c1);

    // Canvas 2
    Canvas* c2 = new Canvas(p1, 450, 0);
    c2->setName("Canvas 2");
    mUi->canvas->scene()->addItem(c2);

    // Common
    TreeModel* model = TreeModel::instance();
    model->setRootItem(mCurrentBlueprint);

    mUi->treeView->setModel(model);
    model->addItem(p1, mCurrentBlueprint);
    model->addItem(c1, p1);
    model->addItem(c2, p1);

    // FIXME this code is so fragile that it should be deleted very quickly
    // it works only if the parent Canvas is selected in the treeview...
    connect(model, &QAbstractItemModel::rowsInserted,
            [this, model](const QModelIndex& parent, int first, int /*last*/) {
        TreeItem* parentItem = model->itemFromIndex(parent);
        TreeItem* childItem = parentItem->child(first);
        mUi->statusBar->showMessage(QString("Created item ") + childItem->name());
    });

    connect(mUi->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [this, model] (const QModelIndex& current,const QModelIndex&/* previous*/) {
        TreeItem* item = model->itemFromIndex(current);
        qDebug() << "ITEM NAME" << item->name();
        if (!item->parentTreeItem()) {
            return;
        }
        model->selectionsChanged(((QModelIndex)*item->parentTreeItem()->modelIndex()),
                                             item->parentTreeItem()->indexOf(item),
                                             item->parentTreeItem()->indexOf(item));
    });

    connect(model, &TreeModel::selectionsChanged,
            [this, model](const QModelIndex& parent, int first, int last) {
        TreeItem* parentItem = model->itemFromIndex(parent);
        TreeItem* childItem = parentItem->child(first);
        mUi->treeView->selectionModel()->select(*childItem->modelIndex(),
                                                QItemSelectionModel::ClearAndSelect);
    });

}

MainWindow::~MainWindow()
{
    delete TreeModel::instance();
    delete mUi;
    qDeleteAll(mTools);
}

void MainWindow::initToolbar()
{
    mTools.append(new Tool(Tool::Type::SELECTION, mUi->actionSelection));
    mTools.append(new Tool(Tool::Type::ELLIPSE, mUi->actionCircle));
    mTools.append(new Tool(Tool::Type::LINE, mUi->actionLine));
    mTools.append(new Tool(Tool::Type::RECTANGLE, mUi->actionRectangle));
    mUi->canvas->setTool(Tool::Type::SELECTION);

    for(Tool* tool : mTools) {
        mUi->toolBar->addAction(tool->getAction());
        connect(tool->getAction(), &QAction::triggered, [this, tool]() {
            setTool(tool->getType());
        });
    }
}

void MainWindow::setTool(Tool::Type toolType)
{
    qDebug() << "Selecting tool" << toolType;
    for(Tool* tool : mTools) {
        if (tool->getType() == toolType) {
            tool->setChecked(true);
        } else {
            tool->setChecked(false);
        }
    }
    mUi->canvas->setTool(toolType);
}
