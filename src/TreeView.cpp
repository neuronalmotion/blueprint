#include "TreeView.h"

#include <QKeyEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDebug>

#include "model/Shape.h"
#include "model/ShapeModel.h"

TreeView::TreeView(QWidget* parent)
    : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void TreeView::dragEnterEvent(QDragEnterEvent* event)
{
   blueprint::Shape* shape = shapeFromInternalPos(event->pos());
   if (!shape) {
       return;
   }
   qDebug() << "Starting drag of" << shape->name();
   mDraggedShape = shape;
   QTreeView::dragEnterEvent(event);
}

void TreeView::dropEvent(QDropEvent* event)
{
   blueprint::Shape* shape = shapeFromInternalPos(event->pos());
   if (!shape || shape == mDraggedShape || !mDraggedShape) {
       return;
   }
   blueprint::ShapeModel* model = blueprint::ShapeModel::instance();
   qDebug() << "Dropping on" << shape->name();
   // for now we consider that we always drag and drop
   // without reparenting (always at the same level)
   blueprint::Shape* parentShape = shape->parentShape();
   int destinationIndex = parentShape->indexOf(shape);
   model->moveShape(mDraggedShape, parentShape, destinationIndex);
   mDraggedShape = nullptr;
   QTreeView::dropEvent(event);
}

void TreeView::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
        case Qt::Key_Delete:
        case Qt::Key_Backspace: {
            blueprint::ShapeModel* model = blueprint::ShapeModel::instance();
            if (model->selectedShape()){
                model->removeItem(model->selectedShape());
            }
        }
        break;
        default:
        break;
        }
    }
    QWidget::keyReleaseEvent(event);
}

blueprint::Shape* TreeView::shapeFromInternalPos(const QPoint& pos)
{
   QModelIndex index = indexAt(pos);
   if (!index.isValid()) {
       return nullptr;
   }
   blueprint::ShapeModel* model = blueprint::ShapeModel::instance();
   return model->itemFromIndex(index);
}

