#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QPointF>

namespace blueprint {
class Shape;

class ShapeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    static ShapeModel* instance();

    explicit ShapeModel();
    ~ShapeModel();

    void addItem(Shape* item, Shape* parent = 0);
    void removeItem(Shape* item);
    void selectShape(Shape* shape);
    void clearSelectedShape();
    inline Shape* selectedShape() { return mSelectedShape; }

    Shape* itemFromIndex(const QModelIndex& index) const;
    Shape* itemFromParentIndex(const QModelIndex& parentIndex, int row) const;

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    inline Shape* rootItem() const { return mRootItem; }
    void setRootItem(Shape* rootItem);

signals:
    void shapeAdded(Shape* shape);
    void shapeRemoved(Shape* shape);
    void shapeSelected(Shape* shape);
    void shapePropertiesChanged(Shape* shape);

private:
    Shape* mRootItem;
    Shape* mSelectedShape;
    static ShapeModel* sInstance;
};
}

#endif // TREEMODEL_H
