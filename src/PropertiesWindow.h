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

public slots:
    void shapeSelected(blueprint::Shape* shape);
    void onBackgroundColorClicked();
    void onBackgroundImageClicked();

private:
    Ui::PropertiesWindow *mUi;
    blueprint::Shape* mCurrentItem;
};

#endif // PROPERTIESWINDOW_H
