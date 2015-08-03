#ifndef GRAPHICALITEM_H
#define GRAPHICALITEM_H

#include <QList>
#include <QPointF>
#include <QString>

class GraphicalItem
{
public:
    explicit GraphicalItem(const QPointF& position = QPointF(), GraphicalItem* parentItem = 0);
    ~GraphicalItem();

    void appendChild(GraphicalItem* child);

    GraphicalItem* child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    GraphicalItem* parentItem();

    QString name() const;
    void setName(const QString& name);

private:
    QPointF mPos;
    QString mName;
    QList<GraphicalItem*> mChildItems;
    GraphicalItem* mParentItem;
};

#endif // GRAPHICALITEM_H
