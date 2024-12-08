#ifndef QSERVERLOADER_H
#define QSERVERLOADER_H

#include <QObject>
#include <QSettings>

#include "../../../Common/QUnNetProtokol/qunnetprotokolserver.h"
#include "../../../Common/QPlugins/qserverplugin.h"
#include "../../../Common/QPlugins/QCoffeePlugin/qcoffeeserverplugin.h"

#include <QDebug>

class QServerLoader : public QObject
{
    Q_OBJECT
public:
    explicit QServerLoader(QObject *parent = nullptr);
    explicit QServerLoader(QSettings *settings,QObject *parent = nullptr);
    ~QServerLoader();

    void startServer();
    void stopServer();

    void setUnDBSettings(QUnDataBaseSettings settingsUnDB);
    void setServerPort(int port);

    void setEnableQdebugMessage(bool in);

private:
    bool flagQDebug = false;
    QSettings *currentSettings;
    int currentServerPort;
    QUnDataBaseSettings currentSettingsUnDB;
    QUnNetProtokolServer *server;

    QVector<QServerPlugin*> pluginList;
    void initPlugins();
    void destroyPlugins();

signals:
    void signalServerStarted();
    void signalServerStoped();
    void signalDebugMessage(QString message);

private slots:
    void slotNewConnection(QUnClientHandler *client);
    void slotClientDisconnected();
    void slotNewExtData (quint8 plugin, quint8 commend,quint16 idBusiness,QByteArray data, QUnClientHandler *client);


    void slotServerStarted();
    void slotServerStoped();
    void slotDebugMessage(QString message);
    void slotPluginDebugMessage(quint8 plugCode,QString plugName, quint16 business, QString message);

public slots:
};

#endif // QSERVERLOADER_H
