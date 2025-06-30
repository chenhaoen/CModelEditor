#include "MainWindow.h"

#include <QApplication>

int main(int argc, char** argv)
{
    // 禁用高DPI缩放
    //qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");

    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    const int result = app.exec();

    return result;
}