#ifndef QUNDBPACKETMANAGER_H
#define QUNDBPACKETMANAGER_H

#include <QObject>
#include <QThread>
#include <QSqlQuery>
#include <QEventLoop>
#include <QDebug>

#include "qunpacketmanager.h"

struct PacketInfo;

class QUnDBPacketManager : public QThread
{
    Q_OBJECT
public:
    QUnDBPacketManager(QString programName, QObject *parent = NULL);
    ~QUnDBPacketManager();
    QEventLoop *mainLoop;
    void addPacket(PacketInfo packet);
    QVector<PacketInfo> getPackets (int idUser);
    QVector<PacketInfo> getPackets ();
    bool deletePacket (PacketInfo packet);

private:
    bool work = false;

    QVector<PacketInfo> packets;
    QMutex *mutexPacket;
    void run();
    QString currentProgramName;
    QMutex *mutexQuery;
    QSqlDatabase dataBase;

    void initDataBase();
    bool insertCommand(PacketInfo packet);

    QSqlQuery *execDebugQuery (QString textQuery, bool *result,bool doCommit = false);
    QSqlQuery *execDebugQuery (QString textQuery, QString nameBindValue, QByteArray &dataBindValue,bool *result,bool doCommit = false);
    void closeDB();

signals:
    void signalDebugMessage (QString message);
};

#endif // QUNDBPACKETMANAGER_H
