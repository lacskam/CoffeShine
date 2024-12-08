#include <QGuiApplication>
#include "startapp.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Homer");
    QCoreApplication::setApplicationName("CoffeeShineClient");

    QGuiApplication app(argc, argv);

#ifdef Q_OS_ANDROID
     qApp->setQuitOnLastWindowClosed(false);  // не должно закрываться при сворачивании
#endif

    StartApp startApp;

    return app.exec();
}
