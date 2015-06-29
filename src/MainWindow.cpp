#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>

#include "CanvasView.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);
    mScene = new QGraphicsScene(this);

    mUi->canvas->setScene(mScene);

    connect(mUi->canvas, &CanvasView::signalMouseReleaseEvent, this, &MainWindow::onCanvasMouseReleaseEvent);
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::onCanvasMouseReleaseEvent(QPointF point)
{
    mScene->addEllipse(point.x(), point.y(), 50, 50);
}
