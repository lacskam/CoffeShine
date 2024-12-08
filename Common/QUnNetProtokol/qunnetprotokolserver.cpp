#include "qunnetprotokolserver.h"

QUnNetProtokolServer::QUnNetProtokolServer(QUnDataBaseSettings settingsDB_,QSettings *settings_,QObject *parent) : QObject(parent)
{
    emit signalDebugMessage("QUnNetProtokolServer created");
    settingsDB = settingsDB_;
    settings = settings_;
    loadSettings();
    initServer();
}

QUnNetProtokolServer::QUnNetProtokolServer(QUnDataBaseSettings settingsDB_,int port, QSettings *settings_, QObject *parent) : QObject(parent)
{
    emit signalDebugMessage("QUnNetProtokolServer created");
    settingsDB = settingsDB_;
    settings = settings_;
    loadSettings();
    initServer();
    currentServerPort = port;
}

QUnNetProtokolServer::~QUnNetProtokolServer()
{
    if (serverSocket->isListening()) stop();
    delete serverSocket;

    dataBase.close();
    QSqlDatabase::removeDatabase("serverUnDB");
}

void QUnNetProtokolServer::initServer()
{
    emit signalDebugMessage("Init server, port: "+QString::number(currentServerPort));
    mutexServer = new QMutex;

    dataBase = QSqlDatabase::addDatabase(settingsDB.type,"serverUnDB");
    dataBase.setConnectOptions("CLIENT_COMPRESS");
    dataBase.setHostName(settingsDB.host);
    dataBase.setDatabaseName(settingsDB.dataBaseName);
    dataBase.setPort(settingsDB.port);
    dataBase.setUserName(settingsDB.login);
    dataBase.setPassword(settingsDB.password);
    qDebug()<<"Open database (init server)"<<dataBase.open();

    accounts = new QUnAccountManager(&dataBase,this);

    /*
    UNAccountInfo A;
    A.login = "homer";
    A.password = "12345";
    A.email = "homer2000@mail.ru";
    A.number = "+375 (25) 705-26-52";
    A.name = "Андрей";
    A.patronymic = "Владимирович";
    A.surname = "Сахарук";

    qDebug()<<"Create new account "<<accounts->createAccount(A);

    qDebug()<<"indexOf="<<accounts->indexOf("homer");
    */

    /*
    UNAccountInfo input = accounts->getAccountInfo(1);
    qDebug()<<"Input account:";
    qDebug()<<input.toString();

    QByteArray buf = input.toByteArray();

    UNAccountInfo Output;
    Output.fromByteArray(buf);
    qDebug()<<"Output account:";
    qDebug()<<Output.toString();
    */

    serverSocket = new QTcpServer;
    connect(serverSocket,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    connect(serverSocket,SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(slotSocketError(QAbstractSocket::SocketError)));
}

void QUnNetProtokolServer::loadSettings()
{
    if (settings!=NULL)
    {
        currentServerPort = settings->value("/QUnNetProtokolServer/currentPort",4444).toInt();
    }
}

void QUnNetProtokolServer::saveSettings()
{
    if (settings!=NULL)
    {
        settings->setValue("/QUnNetProtokolServer/currentPort",currentServerPort);
    }
}

void QUnNetProtokolServer::start()
{
    if (serverSocket->listen(currentAddress,currentServerPort))
    {
        emit signalStartServer();
    }else emit signalStopServer();

}

void QUnNetProtokolServer::stop()
{
    if (serverSocket->isListening())
    {
        serverSocket->close();

        for (int i=0;i<clientList.length();i++)
        {
            ClientInfo A;
            A= clientList.at(i);
            A.handler->disconnectCLient();
        }
    }
    emit signalStopServer();
}

bool QUnNetProtokolServer::isStart()
{
    return serverSocket->isListening();
}

void QUnNetProtokolServer::slotNewConnection()
{
    emit signalDebugMessage("New client connection");
    ClientInfo A;

    A.handler = new QUnClientHandler(accounts,serverSocket->nextPendingConnection());
    connect(A.handler,SIGNAL(signalDisconnected()),this,SLOT(slotClientDisconnect()));
    connect(A.handler,SIGNAL(signalDebugMessage(QString)),this,SIGNAL(signalDebugMessage(QString)));
    //mutexServer->lock();
    clientList.push_back(A);
    //mutexServer->unlock();

    emit signalNewConnection(A.handler);
}

void QUnNetProtokolServer::slotClientDisconnect()
{
    //mutexServer->lock();
    QUnClientHandler *disconnectedClient = (QUnClientHandler *)QObject::sender();
    ClientInfo A;
    for (int i=0;i<clientList.length();i++)
    {
        A= clientList.at(i);
        if (disconnectedClient == A.handler)
        {
            clientList.remove(i);
            break;
        }
    }

    //mutexServer->unlock();
}

QUnAccountManager *QUnNetProtokolServer::getAccountManager()
{
    return accounts;
}

QVector<ClientInfo> *QUnNetProtokolServer::getClientList()
{
    return &clientList;
}

void QUnNetProtokolServer::slotSocketError(QAbstractSocket::SocketError err)
{
    emit signalDebugMessage("Error socket: "+QString::number((int)err));
    qDebug()<<"Error socket: "+QString::number((int)err);
}
