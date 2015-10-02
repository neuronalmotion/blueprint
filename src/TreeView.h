#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

class TreeView : public QTreeView
{
public:
    explicit TreeView(QWidget* parent = 0);

    void keyReleaseEvent(QKeyEvent* event) override;

signals:

public slots:
};

#endif // TREEVIEW_H
