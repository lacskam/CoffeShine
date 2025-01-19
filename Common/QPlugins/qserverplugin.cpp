#include "qserverplugin.h"

QServerPlugin::QServerPlugin(QUnNetProtokolServer *server, quint16 business, QObject *parent) : QObject(parent)
{
    currentServer = server;
    currentIdBusiness = business;
    connect(currentServer,SIGNAL(signalNewConnection(QUnClientHandler*)),
            this,SLOT(slotNewConnection(QUnClientHandler*)));

    mutexDataBase = new QMutex();


    timerDB = new QTimer(this);
    connect(timerDB,SIGNAL(timeout()),this,SLOT(slotTimerDB()));
    timerDB->start(21600000);
}

QServerPlugin::~QServerPlugin()
{
    qDebug()<<"QServerPlugin::~QServerPlugin";
    deinitExtDataBase();
    delete mutexDataBase;
}

quint8 QServerPlugin::pluginCode()
{
    return currentPluginCode;
}

QString QServerPlugin::pluginName()
{
    return currentPluginName;
}

quint16 QServerPlugin::businessId()
{
    return currentIdBusiness;
}

quint32 QServerPlugin::version()
{
    return currentVersion;
}

bool QServerPlugin::initExtDataBase(QDataBaseSettings &settingsDB)
{
    bool Output = false;

    mutexDataBase->lock();

    currentSettingsDB = settingsDB;
    if (!dataBase.isValid()) {
        connectionNameDB = currentPluginName + "_"
                + QString::number(currentPluginCode) + "_"
                + QString::number(currentIdBusiness);

        switch (currentSettingsDB.type) {
        case QDataBaseSettings::typeQDB2: dataBase = QSqlDatabase::addDatabase("QDB2",connectionNameDB);
            break;
        case QDataBaseSettings::typeQIBASE: dataBase = QSqlDatabase::addDatabase("QIBASE",connectionNameDB);
            break;
        case QDataBaseSettings::typeQMYSQL: dataBase = QSqlDatabase::addDatabase("QMYSQL",connectionNameDB);
            break;
        case QDataBaseSettings::typeQOCI: dataBase = QSqlDatabase::addDatabase("QOCI",connectionNameDB);
            break;
        case QDataBaseSettings::typeQODBC: dataBase = QSqlDatabase::addDatabase("QODBC",connectionNameDB);
            break;
        case QDataBaseSettings::typeQPSQL: dataBase = QSqlDatabase::addDatabase("QPSQL",connectionNameDB);
            break;
        case QDataBaseSettings::typeQSQLITE: dataBase = QSqlDatabase::addDatabase("QSQLITE",connectionNameDB);
            break;
        case QDataBaseSettings::typeQSQLITE2: dataBase = QSqlDatabase::addDatabase("QSQLITE2",connectionNameDB);
            break;
        case QDataBaseSettings::typeQTDS: dataBase = QSqlDatabase::addDatabase("QTDS",connectionNameDB);
            break;

        }

        dataBase.setHostName(currentSettingsDB.host);
        dataBase.setPort(currentSettingsDB.port);
        dataBase.setDatabaseName(currentSettingsDB.dataBaseName);
        dataBase.setUserName(currentSettingsDB.login);
        dataBase.setPassword(currentSettingsDB.password);

        Output = dataBase.open();

    }
    mutexDataBase->unlock();

    if (Output)
        dataBaseIsInit();

    return Output;
}

void QServerPlugin::deinitExtDataBase()
{
    mutexDataBase->lock();
    if (dataBase.isValid()) {
        if (dataBase.isOpen())
            dataBase.close();

        dataBase.removeDatabase(connectionNameDB);
    }
    mutexDataBase->unlock();
}

void QServerPlugin::setBusinessId(quint16 id)
{
    currentIdBusiness = id;
}

void QServerPlugin::slotNewConnection (QUnClientHandler *client)
{
    clientsLists.push_back(client);
    connect(client,SIGNAL(signalNewExtData(quint8,quint8,quint16,QByteArray,QUnClientHandler*)),
            this,SLOT(slotNewExtData(quint8,quint8,quint16,QByteArray,QUnClientHandler*)));
    connect(client,SIGNAL(signalDisconnected()),this,SLOT(slotClientDisconnected()));
}

void QServerPlugin::slotClientDisconnected()
{
    QUnClientHandler *client = (QUnClientHandler *)QObject::sender();
    clientsLists.remove(clientsLists.indexOf(client));
}

void QServerPlugin::slotNewExtData (quint8 plugin, quint8 command,
                                    quint16 idBusiness, QByteArray data,
                                    QUnClientHandler *client)
{
    if ((plugin == currentPluginCode) &&
            (idBusiness = currentIdBusiness))
                processCommand(command,data,client);
}

void QServerPlugin::processCommand(quint8 command, QByteArray data, QUnClientHandler *client)
{
    Q_UNUSED(command)
    Q_UNUSED(data)
    Q_UNUSED(client)
}

bool QServerPlugin::sendExtData(quint8 command,QByteArray &data,QUnClientHandler *client)
{
    bool Output = false;

    if (clientsLists.indexOf(client) >= 0) {
        client->sendExtData(currentPluginCode,
                            command,
                            currentIdBusiness,
                            data, true);

        Output = true;
    }

    return Output;
}

QSqlQuery *QServerPlugin::execQuery(QString textQuery, bool *ok)
{
    QSqlQuery *Output;

    mutexDataBase->lock();

    Output = new QSqlQuery(dataBase);

    if (dataBase.isValid()) {
        if (!dataBase.isOpen())
            dataBase.open();

        if (dataBase.isOpen()) {

            if (Output->exec(textQuery)) {
                if (ok != NULL) *ok = true;

            } else if (ok != NULL) *ok = false;

        } else if (ok != NULL) *ok = false;

    } else if (ok != NULL) *ok = false;


    mutexDataBase->unlock();

    return Output;
}

QSqlQuery *QServerPlugin::execQuery(QString textQuery, QString bindName, QVariant bindValue, bool *ok)
{
    QSqlQuery *Output;

    mutexDataBase->lock();

    Output = new QSqlQuery(dataBase);
    Output->prepare(textQuery);
    Output->bindValue(bindName,bindValue);

    if (dataBase.isValid()) {
        if (!dataBase.isOpen())
            dataBase.open();

        if (dataBase.isOpen()) {

            if (Output->exec()) {
                if (ok != NULL) *ok = true;

            } else if (ok != NULL) *ok = false;

        } else if (ok != NULL) *ok = false;

    } else if (ok != NULL) *ok = false;


    mutexDataBase->unlock();

    return Output;
}

void QServerPlugin::dataBaseIsInit()
{

}

int QServerPlugin::getNextId(QString table, QString idName)
{
    int Output = -1;
     QString textQuery = "select max(" + idName +") from " + table + ";";

    if (table=="tbl_pointSale") {
          textQuery = "select max(" + idName +") from " + table + " where id_pointSale!=255;";
    }

    bool result;

    QSqlQuery *queryNexId = execQuery(textQuery,&result);

    if (Output) {
        if (queryNexId->next())
            Output = queryNexId->value(0).toInt() + 1;
        qDebug()<<Output;
    } else {
        qDebug()<<"Error get next id: "<<queryNexId->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryNexId;

    return Output;
}

void QServerPlugin::debugMessage(QString message)
{
    emit signalPluginDebugMessage(currentPluginCode,
                                  currentPluginName,
                                  currentIdBusiness,
                                  message);
}

void QServerPlugin::slotTimerDB()
{
    mutexDataBase->lock();

    if (dataBase.isValid() && dataBase.isOpen()) {
        debugMessage("Timer close DB");
        dataBase.close();
    }

    mutexDataBase->unlock();
}
