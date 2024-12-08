#ifndef QUNNETPROTOKOLSERVER_H
#define QUNNETPROTOKOLSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QVector>
#include <QDebug>
#include <QSettings>
#include <QMutex>


#include "commonunnetworktypes.h"
#include "qunclienthandler.h"
#include "qunaccountmanager.h"
#include "qundatabasesettings.h"

struct ClientInfo
{
    QUnClientHandler *handler;

};


class QUnNetProtokolServer : public QObject
{
    Q_OBJECT
public:
    explicit QUnNetProtokolServer(QUnDataBaseSettings settingsDB_,QSettings *settings_=NULL,QObject *parent = nullptr);
    explicit QUnNetProtokolServer(QUnDataBaseSettings settingsDB_,int port,QSettings *settings_=NULL,QObject *parent = nullptr);
    ~QUnNetProtokolServer();

    void start();
    void stop();


    QUnAccountManager *getAccountManager();
    QVector<ClientInfo> *getClientList();
    bool isStart();

private:
    QUnDataBaseSettings settingsDB;
    QMutex *mutexServer;
    QVector<ClientInfo> clientList;
    QTcpServer *serverSocket;

    int currentServerPort = 4444;
    QHostAddress currentAddress = QHostAddress::Any;

    QSettings *settings;

    QUnAccountManager *accounts;
    QSqlDatabase dataBase;

    void initServer();
    void loadSettings();
    void saveSettings();

signals:
    void signalStartServer();
    void signalStopServer();
    void signalDebugMessage(QString message);
    void signalNewConnection(QUnClientHandler *client);

public slots:

private slots:
    void slotNewConnection();
    void slotClientDisconnect();
    void slotSocketError(QAbstractSocket::SocketError err);
};

#endif // QUNNETPROTOKOLSERVER_H
