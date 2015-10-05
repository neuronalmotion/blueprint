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
    explicit PropertiesWindow(QWidget *parent = 0);
    ~PropertiesWindow();
    void initSignalSlot();

public slots:
    void shapeSelected(blueprint::Shape* shape);
    void onBackgroundColorClicked();
    void onBackgroundImageClicked();
    void onThicknessValueChanged(int val);

    // ShapeText
    void onTextChanged(const QString& text);
    void displayFontDialog();

private:
    void reset();
    void updateTextProperties();
    void updateShapeTextFromProperties();

private:
    Ui::PropertiesWindow *mUi;
    blueprint::Shape* mCurrentItem;
};

#endif // PROPERTIESWINDOW_H
