#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QGraphicsScene>
#include <QAbstractItemModel>
#include <QPointF>

namespace blueprint {
class Shape;
class Blueprint;

class ShapeModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    enum AddMode {
        NORMAL, // insert at child at 0 and notify
        PARCEL, // appends child and do *not* send any signal
    };

    static ShapeModel* instance();

    explicit ShapeModel();
    ~ShapeModel();

    void deleteBlueprint(QGraphicsScene* scene);
    Blueprint* createBlueprint(QGraphicsScene* scene);
    void loadBlueprint(QGraphicsScene* scene, Blueprint* blueprint);

    void addItem(Shape* item, Shape* parent = 0, AddMode addMode = NORMAL);
    void removeItem(Shape* item);
    void selectShape(Shape* shape);
    void clearSelectedShape();
    inline Shape* selectedShape() { return mSelectedShape; }
    void moveShape(Shape* shape, Shape* destinationParent, int destinationIndex);

    Shape* itemFromIndex(const QModelIndex& index) const;
    Shape* itemFromParentIndex(const QModelIndex& parentIndex, int row) const;

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    Qt::DropActions supportedDropActions() const override;

    inline Shape* rootItem() const { return mRootItem; }
    void setRootItem(Shape* rootItem);

signals:
    void shapeAdded(Shape* shape);
    void shapeRemoved(Shape* shape);
    void shapeSelected(Shape* shape);
    void shapeGeometryChanged(Shape* shape);
    void shapePropertiesChanged(Shape* shape);

private:
    static ShapeModel* sInstance;

    Blueprint* mBlueprint;
    Shape* mRootItem;
    Shape* mSelectedShape;
};
}

#endif // TREEMODEL_H
