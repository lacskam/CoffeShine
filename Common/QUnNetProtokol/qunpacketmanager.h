#ifndef QUNPACKETMANAGER_H
#define QUNPACKETMANAGER_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QDateTime>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSqlError>
#include <QMutex>
#include <QSqlResult>

#include "qundbpacketmanager.h"

#include <QDebug>

class QUnDBPacketManager;

struct PacketInfo
{
    quint8 plugin;
    quint8 command;
    quint16 idBusiness;
    int idUser;
    quint32 CRC;
    uint time;

    QByteArray data;
};

class QUnPacketManager : public QThread
{
    Q_OBJECT
public:
    QUnPacketManager(QString programName,bool noDataBase = false, QObject *parent = NULL);
    ~QUnPacketManager();

    void checkMissedPacket(int idUser);

private:
    QUnDBPacketManager *packetDB;
    QString currentProgramName;
    bool flagWork = false;
    void run();
    bool noDataBase_;


    bool connected = false;
    bool authorized = false;
    bool firstRun = true;
    bool checked = false;

    QVector<PacketInfo> packetLine;
    QMutex *mutexPacketLine;

    bool checkDoubleCommand(PacketInfo packet);


public slots:
    void addPacket(PacketInfo packet,bool save=false);
    void slotConnected();
    void slotAuthorized();
    void slotDisconnected();
    void slotConfirmPacket(quint8 plugin, quint8 command, quint32 idBusiness, quint32 currentCRC);

signals:
    void signalReSendData (quint8 plugin,quint8 command,quint16 idBusiness,QByteArray data);
    void signalEndSyncMissedPacket();
    void signalDebugMessage (QString message);
};

#endif // QUNPACKETMANAGER_H
