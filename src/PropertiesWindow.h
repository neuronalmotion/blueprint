#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include <QWidget>

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
    void selectionsChanged(const QModelIndex& parent, int first, int last);

private:
    Ui::PropertiesWindow *mUi;
};

#endif // PROPERTIESWINDOW_H
