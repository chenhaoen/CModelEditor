#include "QT/MainWindow.h"

#include <QApplication>

#include "FileSystem.h"

int main(int argc, char** argv)
{
    che::FileSystem::setAppDir(argv[0]);

    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    const int result = app.exec();

    return result;
}