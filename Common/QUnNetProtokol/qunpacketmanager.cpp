#include "qunpacketmanager.h"

QUnPacketManager::QUnPacketManager(QString programName, bool noDataBase, QObject *parent) : QThread(parent)
{
    currentProgramName = programName;

    noDataBase_ = noDataBase;
    if (!noDataBase) {
        packetDB = new QUnDBPacketManager(currentProgramName,this);
        packetDB->start();
    }

    mutexPacketLine = new QMutex;
    flagWork = true;
    start();
}

QUnPacketManager::~QUnPacketManager()
{
    delete packetDB;

    terminate();
    wait();

    delete mutexPacketLine;

}

void QUnPacketManager::run()
{
    while(flagWork)
    {
        uint currentTime = QDateTime::currentDateTime().toSecsSinceEpoch();

        mutexPacketLine->lock();
        PacketInfo A;
        if ((packetLine.length()==0)&&firstRun&&checked)
        {
            firstRun = false;
            emit signalEndSyncMissedPacket();
        }

        for (int i=0;i<packetLine.length();i++)
        {
            A = packetLine.at(i);
            if ((currentTime - A.time > 60) && authorized) {
                emit signalReSendData(A.plugin,A.command,A.idBusiness,A.data);
                A.time = QDateTime::currentDateTime().toSecsSinceEpoch();
                packetLine.replace(i,A);
            }

        }

       mutexPacketLine->unlock();

       if (!firstRun) {

           for (int i=0;i<30;i++) {
               QThread::sleep(1);
               if (isInterruptionRequested()) {
                   flagWork = false;
                   break;
               }
           }
       } else {
           QThread::sleep(1);

           if (isInterruptionRequested()) {
               flagWork = false;
           }
       }
    }
}

void QUnPacketManager::slotConnected()
{
    connected = true;
}

void QUnPacketManager::slotAuthorized()
{
    authorized = true;
}

void QUnPacketManager::slotDisconnected()
{
    connected = false;
    authorized = false;
}

void QUnPacketManager::slotConfirmPacket(quint8 plugin,quint8 command,quint32 idBusiness,quint32 currentCRC)
{
    emit signalDebugMessage("QUnPacketManager::slotConfirmPacket");

    mutexPacketLine->lock();

    PacketInfo A;

    for (int i=0;i<packetLine.length();i++)
    {
        A = packetLine.at(i);
        if ((A.plugin == plugin)&&(A.command == command) &&
            (A.idBusiness==idBusiness) && (A.CRC == currentCRC))
        {

            packetLine.remove(i);
            emit signalDebugMessage("Удаление подтвержденного пакета " + QString::number(packetLine.length()));

            emit signalDebugMessage("plugin=" + QString::number(A.plugin,16).right(2) +
                                    " command=" + QString::number(A.command,16).right(2) +
                                    " idBusiness=" + QString::number(A.idBusiness,16).right(2) +
                                    " idUser=" + QString::number(A.idUser,16).right(2) +
                                    " CRC=" + QString::number(A.CRC,16).right(8));

            packetDB->deletePacket(A);
            break;
        }
    }

    mutexPacketLine->unlock();
}

void QUnPacketManager::addPacket(PacketInfo packet,bool save)
{
    emit signalDebugMessage("QUnPacketManager::addPacket");

    emit signalDebugMessage("plugin=" + QString::number(packet.plugin,16).right(2) +
                            " command=" + QString::number(packet.command,16).right(2) +
                            " idBusiness=" + QString::number(packet.idBusiness,16).right(2) +
                            " idUser=" + QString::number(packet.idUser,16).right(2) +
                            " CRC=" + QString::number(packet.CRC,16).right(8));

    mutexPacketLine->lock();

    if (!checkDoubleCommand(packet))
    {
        packet.time = QDateTime::currentDateTime().toSecsSinceEpoch();
        packetLine.push_back(packet);

        if ((save) && (!noDataBase_))
            packetDB->addPacket(packet);
    }

    mutexPacketLine->unlock();
}

bool QUnPacketManager::checkDoubleCommand (PacketInfo packet)
{
    bool Output = false;
    PacketInfo A;
    for (int i=0;i<packetLine.length();i++)
    {
        A = packetLine.at(i);
        if ((A.plugin == packet.plugin)&&(A.command == packet.command) && (A.CRC == packet.CRC))
        {
            Output = true;
            break;
        }
    }

    return Output;
}

void QUnPacketManager::checkMissedPacket(int idUser)
{
    qDebug()<<"checkMissedPacket";
    QVector<PacketInfo> packets = packetDB->getPackets();
    checked = true;
    PacketInfo A;

    mutexPacketLine->lock();
    for (int i=0;i<packets.length();i++) {
        A= packets.at(i);
        packetLine.push_back(A);
    }
    mutexPacketLine->unlock();

    if ((packets.length()==0)&&firstRun) {
        firstRun = false;
        emit signalEndSyncMissedPacket();
    }

}

