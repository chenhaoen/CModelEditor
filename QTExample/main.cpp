#include "MainWindow.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    const int result = app.exec();

    return result;
}