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
#include "io/XmlInputOutput.h"

using namespace blueprint;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCurrentBlueprint(nullptr)
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
    mCurrentBlueprint = new Blueprint();

    Page* p1 = ShapeFactory::createPage();
    p1->setName("Page 1");
    model->setRootItem(p1);
    mScene->addItem(p1->graphicsItem());
    mCurrentBlueprint->addPage(p1);

    Canvas* c1 = ShapeFactory::createCanvas(p1);
    c1->setName("Canvas 1");
    model->addItem(c1, p1);

    Canvas* c2 = ShapeFactory::createCanvas(p1);
    c2->setPos(QPointF(450, 0));
    c2->setName("Canvas 2");
    model->addItem(c2, p1);

    initSignalSlots();
}

MainWindow::~MainWindow()
{
    // remove items *before* cleaning treeitem
    for (auto item : mScene->items()) {
        mScene->removeItem(item);
    }
    delete mCurrentBlueprint;
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

    connect(mUi->actionExport_Shape_to_Image, &QAction::triggered, this, &MainWindow::exportShapeToImage);
    connect(mUi->actionSave_as, &QAction::triggered, this, &MainWindow::saveFile);
    connect(mUi->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);

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

void MainWindow::saveFile()
{
    QString filepath = QFileDialog::getSaveFileName(this,
                                 "Save as...",
                                 QDir::homePath(),
                                "Blueprint project (*.bpt);");

    if (!filepath.isNull()) {
         Parcel* parcel = mCurrentBlueprint->toParcel();

         QFile output(filepath);
         output.open(QIODevice::ReadWrite);
         XmlInputOutput xml;
         xml.write(output, *parcel);
         output.close();
    }
}

void MainWindow::loadFile()
{
    QString filepath = QFileDialog::getOpenFileName(this,
                                 "Load...",
                                 QDir::homePath(),
                                "Blueprint project (*.bpt);");

     if (!filepath.isNull()) {
         QFile input(filepath);
         input.open(QIODevice::ReadOnly);
         XmlInputOutput xml;
         Parcel* parcel = xml.read(input);
         input.close();

         mCurrentBlueprint->fromParcel(*parcel);

         Page* firstPage = mCurrentBlueprint->page(0);
         ShapeModel::instance()->setRootItem(firstPage);
         mScene->addItem(firstPage->graphicsItem());
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
