#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int width = w.width();
    int height = w.height();
    w.move ((QApplication::desktop()->width() - width)/2,(QApplication::desktop()->height() - height)/2);


    return a.exec();
}
