#ifndef QUNCLIENTHANDLER_H
#define QUNCLIENTHANDLER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QMutex>
#include <QTime>
#include <QTimer>

#include "qcrc32.h"
#include "qunaccountmanager.h"
#include "commonunnetworktypes.h"
#include "qunpacketmanager.h"

#define UNheaderData 0xA0
#define headerAudio 0xB0

struct PacketBuffer_
{
    quint16 command;
    quint16 count;
    quint16 idBusiness;
    QByteArray buf;
    QDateTime dateTime;
};

class QUnClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit QUnClientHandler(QUnAccountManager *accManager,QTcpSocket *socket ,QObject *parent = nullptr);
    ~QUnClientHandler();

    void disconnectCLient();
    void sendData (quint8 plugin, quint8 command, quint16 idBusiness, QByteArray data, bool sync = false);
    
    UNAccountInfo *getCurrentAccountInfo();
    int getMaxLengthPacket();

    QUnPacketManager *getPacketManager();
    UNAccountInfo *getUnAccountInfo();

private:
    bool Authorized;
    UNAccountInfo currentAccount;
    QTimer *timerPing;
    quint8 pingInterval = 60; //Интервал пинга в сек.
    QTime *timePing = NULL;
    quint16 currentPingCode;
    bool flagPing = false;
    int testTimeCount = 0;
    QMutex *mutexSendData;

    QUnPacketManager *packetManager;

    QVector<PacketBuffer_> packetList;
    bool addPacketInList (PacketBuffer_ &input);
    QByteArray getPacket(quint16 command);

    void parce();
    void commandHandler(QByteArray data);
    
    void command01(QByteArray data);
    void sendCommand01(quint8 direction ,quint16 pingCode);
    void sendPing();

    void command02(QByteArray data);
    void sendCommand02(quint8 plugin, quint8 command, quint32 idBusiness,quint32 CRC);

    void command03 (QByteArray data); //Авторизация
    void sendCommand03 (bool authFlag, quint16 addCode);

    void sendCommand04 (quint16 command,UnErr err);

    void command05 (QByteArray data); //Запрос данных конкретного пользователя
    void sendCommand05 (UNAccountInfo userInfo);

    QUnAccountManager *accountManager;
    QTcpSocket *client;
    QByteArray inputBuffer;

    QCrc32 *crc32;

    quint32 maxLengthPacket=60000;


    QString byteArrayToString (QByteArray input);

    //Для обработчика пакетов
    QTimer *timerCheckPacket;

signals:
    void signalDisconnected();
    void signalNewData(QByteArray data);
    void signalPingTime(quint16 time);
    void signalDebugMessage(QString message);

    void signalNewExtData (quint8 plugin, quint8 commend,quint16 idBusiness,QByteArray data, QUnClientHandler *client);

    void signalErrorPacket(quint8 plugin,quint8 command);
    void signalConfirmPacket(quint8 plugin,quint8 command, quint32 idBusiness, quint32 currentCRC);
    void signalReceiveAccountInfo(QUnClientHandler *client);

public slots:
    void sendExtData(quint8 plugin, quint8 command, quint16 idBusiness, QByteArray data, bool sync = false);
    void slotReSendData (quint8 plugin,quint8 command,quint16 idBusiness,QByteArray data);

private slots:
    void slotDisconnected();
    void slotReadyRead();

    void slotTimerPing();

    //Для обработчика пакетов
    void slotUpdateTimerCheckPacket();
};

#endif // QUNCLIENTHANDLER_H
