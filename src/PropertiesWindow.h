#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include <QWidget>

#include "model/Shape.h"

namespace Ui {
class PropertiesWindow;
}

class PropertiesWindow : public QWidget
{
    Q_OBJECT

public:

    enum PropertiesState {
        CLEAR,
        SHAPE_SELECTED,
    };

    explicit PropertiesWindow(QWidget *parent = 0);
    ~PropertiesWindow();
    void initSignalSlot();

public slots:
    void shapeSelected(blueprint::Shape* shape);
    void shapeGeometryChanged(blueprint::Shape* shape);
    void onBackgroundColorClicked();
    void onBackgroundImageClicked();
    void onThicknessValueChanged(int val);

    // ShapeText
    void onTextChanged(const QString& text);
    void displayFontDialog();

private:
    void clear();
    void updateTextProperties();
    void updateShapeTextFromProperties();
    void toggleSignalBlocks(bool block);

private:
    Ui::PropertiesWindow *mUi;
    blueprint::Shape* mCurrentItem;
    PropertiesState mState;
};

#endif // PROPERTIESWINDOW_H
