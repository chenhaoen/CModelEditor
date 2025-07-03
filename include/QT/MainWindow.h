#ifndef MAINWINDOW_H
#define MAINWINDOW_H

namespace Ui {
class MainWindow;
}

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_Open_triggered(bool);
    void on_action_Fill_triggered(bool);
    void on_action_Mesh_triggered(bool);
    void on_action_Point_triggered(bool);
    void on_actionSceneShoot_triggered(bool);
    void on_actionCube_triggered(bool);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
