#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void onCanvasMouseReleaseEvent(QPointF point);

private:
    Ui::MainWindow* mUi;
    QGraphicsScene* mScene;
};

#endif // MAINWINDOW_H
