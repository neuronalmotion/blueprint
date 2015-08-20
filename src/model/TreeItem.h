#ifndef TREEITEM_H
#define TREEITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPointF>
#include <QString>
#include <QPersistentModelIndex>
#include <QModelIndex>

namespace blueprint {

class TreeItem
{
public:

    enum ItemType {
        // General types
        BLUEPRINT,
        PAGE,
        CANVAS,
        SHAPE,
    };

    explicit TreeItem(const ItemType& itemType, TreeItem* parentTreeItem = 0);
    virtual ~TreeItem();

    void appendChild(TreeItem* child);

    TreeItem* child(int row);
    int indexOf(const TreeItem* child) const;
    int childCount() const;
    int columnCount() const;
    int row() const;

    inline QString name() const { return mName; }
    inline void setName(const QString& name) { mName = name; }
    inline ItemType itemType() const { return mItemType; }
    inline TreeItem* parentTreeItem() const {return mParentItem; }
    inline void setParentTreeItem(TreeItem* parentItem) { mParentItem = parentItem; }
    inline QPersistentModelIndex* modelIndex() const { return mModelIndex; }
    inline void setModelIndex(const QModelIndex& index) { mModelIndex = new QPersistentModelIndex(index); }

    virtual inline void setSelected(bool selected) { mIsSelected = selected; }
protected:
    QString mName;
    ItemType mItemType;
    TreeItem* mParentItem;
    QList<TreeItem*> mChildItems;
    QPersistentModelIndex* mModelIndex;
    bool mIsSelected;
};
}

#endif // TREEITEM_H
