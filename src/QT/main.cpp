#include "QT/MainWindow.h"

#include <QApplication>

#include "CFileSystem.h"

int main(int argc, char** argv)
{
    CFileSystem::setAppDir(argv[0]);

    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    const int result = app.exec();

    return result;
}