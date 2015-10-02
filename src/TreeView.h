#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>
#include <QPoint>

namespace blueprint {
class Shape;
}

class TreeView : public QTreeView
{
public:
    explicit TreeView(QWidget* parent = 0);

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;


private:
    blueprint::Shape* shapeFromInternalPos(const QPoint& pos);

private:
    blueprint::Shape* mDraggedShape;
};

#endif // TREEVIEW_H
