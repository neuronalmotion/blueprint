#ifndef GRAPHICALITEM_H
#define GRAPHICALITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPointF>
#include <QString>
#include <QPersistentModelIndex>
#include <QModelIndex>


class GraphicalItem
{
public:

    enum GraphicalType {
        // General types
        BLUEPRINT,
        PAGE,
        CANVAS,
        SHAPE,
    };

    explicit GraphicalItem(const GraphicalType& graphicalType, GraphicalItem* parentItem = 0);
    ~GraphicalItem();

    virtual QGraphicsItem* getGraphicsItem() { return nullptr; }

    void appendChild(GraphicalItem* child);

    GraphicalItem* child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;

    inline QString name() const { return mName; }
    inline void setName(const QString& name) { mName = name; }
    inline GraphicalType graphicalType() const { return mGraphicalType; }
    inline GraphicalItem* parentItem() const {return mParentItem; }
    inline void setParent(GraphicalItem* parentItem) { mParentItem = parentItem; }
    inline QPersistentModelIndex* modelIndex() const { return mModelIndex; }
    inline void setModelIndex(const QModelIndex& index) { mModelIndex = new QPersistentModelIndex(index); }

    virtual inline void setSelected(bool selected) { mIsSelected = selected; }
protected:
    QString mName;
    GraphicalType mGraphicalType;
    QList<GraphicalItem*> mChildItems;
    GraphicalItem* mParentItem;
    QPersistentModelIndex* mModelIndex;
    bool mIsSelected;
};

#endif // GRAPHICALITEM_H
