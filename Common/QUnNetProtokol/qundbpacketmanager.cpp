#include "qundbpacketmanager.h"

QUnDBPacketManager::QUnDBPacketManager(QString programName, QObject *parent) :QThread(parent)
{
    currentProgramName = programName;

    mutexQuery = new QMutex;
    mutexPacket = new QMutex;
    initDataBase();
    work = true;
}

QUnDBPacketManager::~QUnDBPacketManager()
{
    work = false;
    mainLoop->exit(0);
    wait();

    delete mutexQuery;
    delete mutexPacket;

}

void QUnDBPacketManager::run()
{
    mainLoop = new QEventLoop();

    while (work) {
        QVector<PacketInfo> buf;
        mutexPacket->lock();
        buf = packets;
        packets.clear();
        mutexPacket->unlock();

        for (int i=0;i<buf.length();i++)
            insertCommand(buf.at(i));

        mainLoop->exec();
    }
}

void QUnDBPacketManager::addPacket(PacketInfo packet)
{
    mutexPacket->lock();
    packets.push_back(packet);
    mainLoop->exit(0);
    mutexPacket->unlock();
}

void QUnDBPacketManager::initDataBase()
{
    QString workDir = QDir::homePath() + "/."+currentProgramName;

    if (!QDir(workDir).exists()) QDir(workDir).mkdir(workDir);

    dataBase = QSqlDatabase::addDatabase("QSQLITE",currentProgramName+"_timeShift");

#ifdef Q_OS_ANDROID
    dataBase.setDatabaseName("timeShift.db");
#else
    dataBase.setDatabaseName(workDir+"/timeShift.db");
#endif

    dataBase.open();

    bool result;
    QSqlQuery *queryCreateTable;
    QString textQuery = "CREATE TABLE IF NOT EXISTS commands ("
                                   "id_command INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                                   "plugin INTEGER NOT NULL,"
                                   "command INTEGER NOT NULL,"
                                   "idBusiness INTEGER NOT NULL,"
                                   "idUser INTEGER NOT NULL,"
                                   "pData blob NOT NULL,"
                                   "date DATETIME NOT NULL,"
                                   "crc INTEGER NOT NULL);";

    queryCreateTable = execDebugQuery(textQuery,&result,true);

    if (!result) {
        qDebug()<<"QUnPacketManager: error create table";
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryCreateTable;
}

bool QUnDBPacketManager::insertCommand(PacketInfo packet)
{
    bool Output;


    QSqlQuery *queryInsert;
    QString textQuery = "insert into commands ("
                         "plugin,"
                         "command,"
                         "idBusiness,"
                         "idUser,"
                         "pData,"
                         "date,"
                         "crc"
                         ") values("
                         "'"+QString::number(packet.plugin)+"',"
                         "'"+QString::number(packet.command)+"',"
                         "'"+QString::number(packet.idBusiness)+"',"
                         "'"+QString::number(packet.idUser)+"',"
                         ":packetData,"
                         "'"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"',"
                         "'"+QString::number(packet.CRC)+"');";

    queryInsert = execDebugQuery(textQuery,":packetData",packet.data,&Output,true);


    if (!Output)
    {
        qDebug()<<"Error add commands:"<<queryInsert->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        emit signalDebugMessage("Error add commands:" + queryInsert->lastError().text());
    }
    delete queryInsert;


    return Output;
}

bool QUnDBPacketManager::deletePacket (PacketInfo packet)
{
    bool Output;

    QSqlQuery *queryDelete = new QSqlQuery(dataBase);
    QString textQuery = "delete from commands where "
                        "plugin = '"+QString::number(packet.plugin)+"' and "
                        "command = '"+QString::number(packet.command)+"' and "
                        "idBusiness = '"+QString::number(packet.idBusiness)+"' and "
                        "idUser = '"+QString::number(packet.idUser)+"' and "
                        "crc = '"+QString::number(packet.CRC)+"';";

    queryDelete = execDebugQuery(textQuery,&Output);

    if (!Output)
    {
        qDebug()<<"Error delete command:"<<queryDelete->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        emit signalDebugMessage("Error delete command:" + queryDelete->lastError().text());
    }

    delete queryDelete;


    return Output;
}

QVector<PacketInfo> QUnDBPacketManager::getPackets (int idUser)
{
    QVector<PacketInfo>  Output;

    QString textQuery = "select "
                        "plugin,"
                        "command,"
                        "idBusiness,"
                        "idUser,"
                        "pData,"
                        "date,"
                        "crc "
                        "from commands where "
                        "idUser = '"+QString::number(idUser)+"';";

    QSqlQuery *queryPackets;
    bool result;
    queryPackets = execDebugQuery(textQuery,&result);

    if (result)
    {
        PacketInfo A;
        while (queryPackets->next())
        {
            A.plugin = queryPackets->value(0).toInt();
            A.command = queryPackets->value(1).toInt();
            A.idBusiness = queryPackets->value(2).toInt();
            A.idUser = queryPackets->value(3).toInt();
            A.data = queryPackets->value(4).toByteArray();
            A.time = QDateTime::fromString(queryPackets->value(5).toString(),"yyyy-MM-dd hh:mm;ss").toSecsSinceEpoch();
            A.CRC = queryPackets->value(6).toInt();

            Output.push_back(A);
        }
    }
     else
    {
        qDebug()<<"Error get packets: "<<queryPackets->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        emit signalDebugMessage("Error get packets: " + queryPackets->lastError().text());
    }

    delete queryPackets;



    return Output;
}

QVector<PacketInfo> QUnDBPacketManager::getPackets ()
{
    QVector<PacketInfo>  Output;

    QString textQuery = "select "
                        "plugin,"
                        "command,"
                        "idBusiness,"
                        "idUser,"
                        "pData,"
                        "date,"
                        "crc "
                        "from commands;";

    QSqlQuery *queryPackets;
    bool result;
    queryPackets = execDebugQuery(textQuery,&result);

    if (result)
    {
        PacketInfo A;
        while (queryPackets->next())
        {
            A.plugin = queryPackets->value(0).toInt();
            A.command = queryPackets->value(1).toInt();
            A.idBusiness = queryPackets->value(2).toInt();
            A.idUser = queryPackets->value(3).toInt();
            A.data = queryPackets->value(4).toByteArray();
            A.time = QDateTime::fromString(queryPackets->value(5).toString(),"yyyy-MM-dd hh:mm;ss").toSecsSinceEpoch();
            A.CRC = queryPackets->value(6).toInt();

            Output.push_back(A);
        }
    }
     else
    {
        qDebug()<<"Error get packets: "<<queryPackets->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        emit signalDebugMessage("Error get packets: " + queryPackets->lastError().text());
    }

    delete queryPackets;



    return Output;
}

QSqlQuery *QUnDBPacketManager::execDebugQuery (QString textQuery, bool *result, bool doCommit)
{
    mutexQuery->lock();
    QSqlQuery *Output;
    bool y;
    if (!dataBase.isOpen())
        dataBase.open();

    Output = new QSqlQuery(dataBase);
    if (result!=NULL) {
        *result = Output->exec(textQuery);
    } else {
        y = Output->exec(textQuery);
    }


    if (doCommit) {
        dataBase.commit();
    }

    mutexQuery->unlock();
    return Output;
}

QSqlQuery *QUnDBPacketManager::execDebugQuery (QString textQuery, QString nameBindValue, QByteArray &dataBindValue,bool *result,bool doCommit)
{
    mutexQuery->lock();
    QSqlQuery *Output;
    bool y = false;
    if (!dataBase.isOpen())
        dataBase.open();

    Output = new QSqlQuery(dataBase);

    Output->prepare(textQuery);
    Output->bindValue(nameBindValue,dataBindValue);


    if (result!=NULL) {
        *result = Output->exec();
    } else {
        y = Output->exec();
    }


    if (doCommit) {
        dataBase.commit();
    }

    mutexQuery->unlock();
    return Output;
}

void QUnDBPacketManager:: closeDB()
{
    mutexQuery->lock();
    dataBase.close();
    mutexQuery->unlock();
}
