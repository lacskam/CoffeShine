#include "qclientplugin.h"

QClientPlugin::QClientPlugin(QUnNetProtokolCLient *client,quint16 business, QObject *parent) : QObject(parent)
{
    dataBase = NULL;
    currentClient = client;
    currentIdBusiness = business;
    mutexDataBase = new QMutex;

    connect(currentClient,SIGNAL(signalNewExtPacket(quint8,quint8,quint16,QByteArray)),
            this,SLOT(slotNewExtPacket(quint8,quint8,quint16,QByteArray)));
}

QClientPlugin::~QClientPlugin()
{
    deinitDataBase();
}

void QClientPlugin::startSynchronization()
{

}

quint8 QClientPlugin::pluginCode()
{
    return currentPluginCode;
}

QString QClientPlugin::pluginName()
{
    return currentPluginName;
}

quint16 QClientPlugin::businessId()
{
    return currentIdBusiness;
}

quint32 QClientPlugin::version()
{
    return currentVersion;
}

bool QClientPlugin::initDataBase()
{
    bool Output = false;

    mutexDataBase->lock();

    if (dataBase == NULL) {
        dataBase = new QSqlDatabase();
        connectionNameDB = currentPluginName + "_"
                + QString::number(currentPluginCode) + "_"
                + QString::number(currentIdBusiness);

        *dataBase = QSqlDatabase::addDatabase("QSQLITE",connectionNameDB);
        dataBase->setDatabaseName(":memory:");

        Output = dataBase->open();
    }

    mutexDataBase->unlock();

    if (Output)
        dataBaseIsInit();

    return Output;
}

void QClientPlugin::deinitDataBase()
{
    mutexDataBase->lock();
    if (dataBase != NULL) {
        if (dataBase->isOpen())
            dataBase->close();

        delete dataBase;
        QSqlDatabase::removeDatabase(connectionNameDB);
    }

    mutexDataBase->unlock();
}

QSqlQuery *QClientPlugin::execQuery(QString textQuery, bool *ok)
{
    QSqlQuery *Output;

    mutexDataBase->lock();

    Output = new QSqlQuery(*dataBase);

    if (dataBase !=NULL) {
        if (dataBase->isValid()) {
            if (!dataBase->isOpen())
                dataBase->open();

            if (dataBase->isOpen()) {

                if (Output->exec(textQuery)) {
                    if (ok != NULL) *ok = true;

                } else if (ok != NULL) *ok = false;

            } else if (ok != NULL) *ok = false;

        } else if (ok != NULL) *ok = false;
    }


    mutexDataBase->unlock();

    return Output;
}

int QClientPlugin::getNextId(QString table, QString idName)
{
    int Output = -1;

    QString textQuery = "select max(" + idName +") from " + table + ";";
    bool result;

    QSqlQuery *queryNexId = execQuery(textQuery,&result);

    if (Output) {
        if (queryNexId->next()) {
            Output = queryNexId->value(0).toInt() + 1;
            if (Output < startId)
                Output = startId;
        }
    } else {
        qDebug()<<"Error get next id: "<<queryNexId->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryNexId;

    return Output;
}


void QClientPlugin::processCommand(quint8 command,QByteArray data)
{
    Q_UNUSED(command)
    Q_UNUSED(data)
}

bool QClientPlugin::sendExtData(quint8 command, QByteArray &data)
{
    bool Output = false;

        currentClient->sendExtData(currentPluginCode,command,data,currentIdBusiness,true);
        Output = true;

    return Output;

}

void QClientPlugin::dataBaseIsInit()
{

}

void QClientPlugin::debugMessage(QString message)
{
    emit signalPluginDebugMessage(currentPluginCode,
                                  currentPluginName,
                                  currentIdBusiness,
                                  message);
}

void QClientPlugin::slotNewExtPacket (quint8 plugin,quint8 command, quint16 idBusiness, QByteArray data)
{
    if ((plugin == currentPluginCode) &&
            (idBusiness = currentIdBusiness))
                processCommand(command,data);
}

QStringList QClientPlugin::getDebugTableist()
{
    QStringList Output;
    mutexDataBase->lock();

    if (dataBase->isValid())
        Output = dataBase->tables();

    mutexDataBase->unlock();
    return Output;
}

QStringList QClientPlugin::getTableHeader(QString tableName)
{
    QStringList Output;
    mutexDataBase->lock();

    if (dataBase->isValid())
    {
        QSqlRecord currentRecord = dataBase->record(tableName);
        int countFields = currentRecord.count();
        for (int i=0;i<countFields;i++)
            Output<<currentRecord.fieldName(i);
    }

    mutexDataBase->unlock();

    return Output;
}
