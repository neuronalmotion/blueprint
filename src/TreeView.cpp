#include "TreeView.h"

#include <QKeyEvent>

#include "model/ShapeModel.h"

using namespace blueprint;

TreeView::TreeView(QWidget* parent)
    : QTreeView(parent)
{
}

void TreeView::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
        case Qt::Key_Delete:
        case Qt::Key_Backspace: {
            ShapeModel* model = ShapeModel::instance();
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

