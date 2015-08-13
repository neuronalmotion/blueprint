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

    void addGraphicalItem(GraphicalItem* item, GraphicalItem* parent = 0, const QModelIndex& parentIndex = QModelIndex());
    GraphicalItem* graphicalItemFromIndex(const QModelIndex& index) const;

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:

private:
    GraphicalItem* mRootItem;
};

#endif // GRAPHICALMODEL_H
