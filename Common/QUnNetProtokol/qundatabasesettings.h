#ifndef QUNDATABASESETTINGS_H
#define QUNDATABASESETTINGS_H

#include <QObject>

class QUnDataBaseSettings
{
public:
    QUnDataBaseSettings();

    QString type;
    QString host;
    QString dataBaseName;
    int port;
    QString login;
    QString password;
};

#endif // QUNDATABASESETTINGS_H
