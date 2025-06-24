#include "MainWindow.h"
#include "SceneWindow.h"

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWindow *sceneWindow = new SceneWindow();
    QWidget *wrapper = QWidget::createWindowContainer(sceneWindow,this);
    setCentralWidget(wrapper);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Fill_triggered(bool)
{

}

void MainWindow::on_action_Mesh_triggered(bool)
{
}

void MainWindow::on_action_Point_triggered(bool)
{
}

void MainWindow::on_actionSceneShoot_triggered(bool)
{
}

void MainWindow::on_actionCube_triggered(bool)
{
}

void MainWindow::on_action_Open_triggered(bool)
{
}