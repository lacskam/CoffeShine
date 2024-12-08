#include "include/QDesktopClientLoader/qdesktopclientloader.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopClientLoader w;

    return a.exec();
}
