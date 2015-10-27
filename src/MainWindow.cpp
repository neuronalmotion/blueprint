#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QWidget>
#include <QKeyEvent>
#include <QImage>
#include <QPainter>
#include <QFileDialog>

#include "CanvasView.h"
#include "model/Blueprint.h"
#include "model/Page.h"
#include "model/Canvas.h"
#include "model/Shape.h"
#include "model/BezierPoint.h"
#include "model/BezierControlPoint.h"
#include "model/ShapeModel.h"
#include "model/ShapeFactory.h"
#include "utils/FileUtils.h"

using namespace blueprint;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mBlueprint(nullptr)
{
    mUi->setupUi(this);
    mUi->canvas->setRenderHints(QPainter::Antialiasing
                                | QPainter::TextAntialiasing);
    resize(1200,700);

    initToolbar();

    // Warning! The scene must be owned by MainWindow
    // and deleted manually (no Qt parenting).
    // All drawn TreeItems (Shapes) are derived classes
    // of QGraphicsItem, which would delete our items before us
    mScene = new QGraphicsScene();
    ShapeModel* model = ShapeModel::instance();
    mUi->canvas->setScene(mScene);
    mUi->treeView->setModel(model);


    // Init default Blueprint project
    mBlueprint = model->createBlueprint(mScene);

    Page* p1 = mBlueprint->activePage();
    Canvas* c2 = ShapeFactory::createCanvas(p1);
    c2->setPos(QPointF(450, 0));
    c2->setName("Canvas 2");
    model->addItem(c2, p1);

    initSignalSlots();
}

MainWindow::~MainWindow()
{
    ShapeModel* model = ShapeModel::instance();
    model->deleteBlueprint(mScene);
    delete ShapeModel::instance();
    delete mUi;
    qDeleteAll(mTools);
}

void MainWindow::initToolbar()
{
    mTools.append(new Tool(Tool::Type::SELECTION, mUi->actionSelection));
    mTools.append(new Tool(Tool::Type::ELLIPSE, mUi->actionCircle));
    mTools.append(new Tool(Tool::Type::LINE, mUi->actionLine));
    mTools.append(new Tool(Tool::Type::RECTANGLE, mUi->actionRectangle));
    mTools.append(new Tool(Tool::Type::TEXT, mUi->actionText));
    mUi->canvas->setTool(Tool::Type::SELECTION);

    for(Tool* tool : mTools) {
        mUi->toolBar->addAction(tool->getAction());
        connect(tool->getAction(), &QAction::triggered, [this, tool]() {
            setTool(tool->getType());
        });
    }
}

void MainWindow::initSignalSlots()
{
    ShapeModel* model = ShapeModel::instance();

    connect(mUi->actionNew_Project, &QAction::triggered, this, &MainWindow::newBlueprint);
    connect(mUi->actionLoad, &QAction::triggered, this, &MainWindow::loadBlueprint);
    connect(mUi->actionSave, &QAction::triggered, [=](){ this->saveBlueprint(SAVE_IF_POSSIBLE); });
    connect(mUi->actionSave_as, &QAction::triggered, [=](){ this->saveBlueprint(FORCE_SAVE_AS); });
    connect(mUi->actionExport_Shape_to_Image, &QAction::triggered, this, &MainWindow::exportShapeToImage);

    connect(model, &ShapeModel::shapeAdded,
            [this](Shape* shape) {
        mUi->statusBar->showMessage(QString("Created shape ") + shape->name());
    });

    connect(mUi->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [this, model] (const QModelIndex& current,const QModelIndex&/* previous*/) {
        Shape* item = model->itemFromIndex(current);
        if (!item->parentShape()) {
            return;
        }
        model->selectShape(item);
    });

    connect(model, &ShapeModel::shapeSelected,
            [this](Shape* shape) {
        if (!shape) {
            mUi->treeView->selectionModel()->clearSelection();
        } else {
            mUi->treeView->selectionModel()->select(*shape->modelIndex(),
                                                QItemSelectionModel::ClearAndSelect);
        }
    });

    connect(model, &ShapeModel::shapeRemoved,
            [this](Shape* shape) {
        mUi->statusBar->showMessage(QString("Deleted shape ") + shape->name());
        mScene->removeItem(shape->graphicsItem());
        delete shape;
    });

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

void MainWindow::exportShapeToImage()
{
    ShapeModel* model = ShapeModel::instance();
    QString filepath = QFileDialog::getSaveFileName(this,
                                 "Export Shape to Image",
                                 QDir::homePath(),
                                "JPG (*.jpg);;PNG (*.png)");
    if (!filepath.isNull()) {
        Shape* shape = model->selectedShape();
        if (!shape) {
            shape = ShapeModel::instance()->rootItem();
        }
        saveShapeToImage(*shape, filepath);
    }
}

void MainWindow::newBlueprint()
{
    // TODO detect if current project is modified, if yes, display a save pop up
    mBlueprint = ShapeModel::instance()->createBlueprint(mScene);
}

void MainWindow::saveBlueprint(SaveAction saveAction)
{
    QString filepath;
    if (saveAction == FORCE_SAVE_AS || !mBlueprint->hasFile()) {
        filepath = QFileDialog::getSaveFileName(this,
                                                "Save as...",
                                                QDir::homePath(),
                                                "Blueprint project (*.bpt)");


    } else {
        filepath = mBlueprint->file()->fileName();
    }
    if (!filepath.isNull()) {
         FileUtils::saveBlueprintToFile(*mBlueprint, filepath);
        mUi->statusBar->showMessage(QString("Project saved to ") + filepath);
         mBlueprint->setFile(filepath);
    }
}

void MainWindow::loadBlueprint()
{
    QString filepath = QFileDialog::getOpenFileName(this,
                                                    "Load...",
                                                    QDir::homePath(),
                                                    "Blueprint project (*.bpt)");

    if (!filepath.isNull()) {
        Blueprint* blueprint = FileUtils::loadBlueprintFromFile(filepath);
        if (blueprint) {
            ShapeModel::instance()->loadBlueprint(mScene, blueprint);
            mBlueprint = blueprint;
        }
    }
}

void MainWindow::saveShapeToImage(Shape& shape, const QString& filepath)
{
    qDebug() << "Saving shape to" << filepath;
    ShapeModel* model = ShapeModel::instance();
    Shape* selectedShape = model->selectedShape();
    model->clearSelectedShape();

    QGraphicsItem* graphicsItem = shape.graphicsItem();
    QRectF srcRect = graphicsItem->sceneBoundingRect();
    QRectF dstRect = QRectF(QPointF(0, 0), srcRect.size());

    QImage image(srcRect.width(), srcRect.height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    mScene->render(&painter, dstRect, srcRect);
    painter.end();
    image.save(filepath);

    model->selectShape(selectedShape);
}
