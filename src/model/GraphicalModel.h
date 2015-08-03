#ifndef GRAPHICALMODEL_H
#define GRAPHICALMODEL_H

#include <QAbstractItemModel>
#include <QPointF>

class GraphicalItem;

class GraphicalModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GraphicalModel(GraphicalItem* rootItem = 0, QObject* parent = 0);
    ~GraphicalModel();

    void addGraphicalItem(GraphicalItem* item, GraphicalItem* parent = 0);

    QVariant data(const QModelIndex& index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

private:
    GraphicalItem* graphicalItemFromIndex(const QModelIndex& index) const;

private:
    GraphicalItem* mRootItem;
};

#endif // GRAPHICALMODEL_H
