#include "qserverloader.h"

QServerLoader::QServerLoader(QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(signalDebugMessage(QString)),this,SLOT(slotDebugMessage(QString)));
}

QServerLoader::QServerLoader(QSettings *settings,QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(signalDebugMessage(QString)),this,SLOT(slotDebugMessage(QString)));
    currentSettings = settings;
}

QServerLoader::~QServerLoader()
{

}

void QServerLoader::startServer()
{
    server = new QUnNetProtokolServer(currentSettingsUnDB,currentServerPort,currentSettings,this);
    connect(server,SIGNAL(signalStartServer()),this,SLOT(slotServerStarted()));
    connect(server,SIGNAL(signalStopServer()),this,SLOT(slotServerStoped()));
    connect(server,SIGNAL(signalNewConnection(QUnClientHandler*)),this,SLOT(slotNewConnection(QUnClientHandler*)));

    server->start();
}

void QServerLoader::stopServer()
{
    server->stop();

    delete server;
}

void QServerLoader::setUnDBSettings(QUnDataBaseSettings settingsUnDB)
{
    currentSettingsUnDB = settingsUnDB;
}

void QServerLoader::setServerPort(int port)
{
    currentServerPort = port;
}

void QServerLoader::slotNewConnection(QUnClientHandler *client)
{
    emit signalDebugMessage(tr("CLient connected: ")+client->getCurrentAccountInfo()->login);
    connect(client,SIGNAL(signalNewExtData(quint8,quint8,quint16,QByteArray,QUnClientHandler*)),
            this,SLOT(slotNewExtData(quint8,quint8,quint16,QByteArray,QUnClientHandler*)));
    connect(client,SIGNAL(signalDisconnected()),this,SLOT(slotClientDisconnected()));
}

void QServerLoader::slotClientDisconnected()
{
    QUnClientHandler *client = (QUnClientHandler*)QObject::sender();
    emit signalDebugMessage(tr("CLient disconnected: ")+client->getCurrentAccountInfo()->login);
}

void QServerLoader::slotNewExtData (quint8 plugin, quint8 commend,quint16 idBusiness,QByteArray data, QUnClientHandler *client)
{
    Q_UNUSED(plugin)
    Q_UNUSED(commend)
    Q_UNUSED(idBusiness)
    Q_UNUSED(data)
    Q_UNUSED(client)
}

void QServerLoader::slotServerStarted()
{
    emit signalServerStarted();
    initPlugins();
}

void QServerLoader::slotServerStoped()
{
    emit signalServerStoped();
    destroyPlugins();
}

void QServerLoader::initPlugins()
{
    QServerPlugin *currentPlugin;

    currentPlugin = new QCoffeeServerPlugin(server,0x06,this);
    connect(currentPlugin,SIGNAL(signalPluginDebugMessage(quint8,QString,quint16,QString)),
            this,SLOT(slotPluginDebugMessage(quint8,QString,quint16,QString)));
    pluginList.push_back(currentPlugin);

    QDataBaseSettings dbSettings;
    dbSettings.type = QDataBaseSettings::typeQMYSQL;
    dbSettings.dataBaseName = "bd";
    dbSettings.host = "127.0.0.1";
    dbSettings.login = "root";
    dbSettings.password = "1756";
    dbSettings.port = 3306;

    qDebug()<<"Init external database: "<<currentPlugin->initExtDataBase(dbSettings);


}

void QServerLoader::destroyPlugins()
{
    for (int i=0;i<pluginList.length();i++)
        delete pluginList.at(i);

    pluginList.clear();
}

void QServerLoader::setEnableQdebugMessage(bool in)
{
    flagQDebug = in;
}

void QServerLoader::slotDebugMessage(QString message)
{
    if (flagQDebug)
        qDebug()<<message;
}

void QServerLoader::slotPluginDebugMessage(quint8 plugCode,
                                           QString plugName,
                                           quint16 business,
                                           QString message)
{
    Q_UNUSED(plugCode)
    emit signalDebugMessage(plugName + "(" + QString::number(business) + "):" + message);

}
