#ifndef QUNNETPROTOKOLCLIENT_H
#define QUNNETPROTOKOLCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include <QVector>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QTimer>
#include <QMutex>
#include <QDateTime>
#include <QMutex>

#include "commonunnetworktypes.h"
#include "qcrc32.h"
#include "qunaccountmanager.h"

#include "qunpacketmanager.h"
#include <QTime>
#include <QDebug>
#include <QElapsedTimer>
#define UNheaderData 0xA0
#define headerAudio 0xB0

struct PacketBuffer
{
    quint16 command;
    quint16 count;
    quint16 idBusiness;
    QByteArray buf;
    QDateTime dateTime;
    quint32 CRC;
};

class QUnNetProtokolCLient : public QObject
{
    Q_OBJECT
public:
    explicit QUnNetProtokolCLient(QString programName, QObject *parent = nullptr);
    explicit QUnNetProtokolCLient(QString programName,QSettings *settings_, QObject *parent = nullptr);
    ~QUnNetProtokolCLient();

    QUnPacketManager *packetManager;

    bool isConnected ();
    bool isAuthorizes ();

    bool connectToHost ();
    void disconnectFromHost ();
    QVector<HostInfo> hosts ();
    int countHosts ();

    int currentHostIndex();
    void deleteHost(int index);
    void editHost(int index,HostInfo &host);

    QString getLogin ();
    QString getPassword ();
    UNAccountInfo getCurrentAccountInfo ();
    int getMaxLengthPacket ();

    void setAccount (QString login,QString password);
    void setHosts (QVector<HostInfo> hosts_);
    void setHost (HostInfo &host);
    void sendData (quint8 plugin, quint8 command, quint16 idBusiness, QByteArray data, bool save = false);
    void sendExtData (quint8 plugin,quint8 command, QByteArray data, quint16 idBusiness, bool save = false);
    void setSaveLogin (bool input);
    void setSavePassword (bool input);
    void setTimeConnection(quint8 time);
    void setTimeAuthorize(quint8 time);

    bool waitForDisconnected(int msecs = 30000);


private:
    QString currentProgramName;
    QTimer *timerCheck;
    QTcpSocket *socket;
    QThread *thrSocket;
    QTimer *timerPing;
    quint8 pingInterval = 60; //Интервал пинга в сек.
    QElapsedTimer *timePing = NULL;
    quint16 currentPingCode;
    bool flagPing = false;
    void sendPing();
    int countDebugPing =0;

    QVector<PacketBuffer> packetList;
    bool addPacketInList (PacketBuffer &input);
    QByteArray getPacket(quint16 command, quint32 idBusiness);

    void initClient();
    void loadSettings();
    void saveSettings();
    bool connectionFlag;


    QVector<HostInfo> hostsList;
    QSettings *settings;
    int currentHostNumber;
    QTimer *timerReconnect;

    QCrc32 *crc32;
    int getNextHostNumber (int number);

    QString currentLogin;
    QString currentPassword;
    bool saveLogin = false;
    bool savePassword = false;

    bool logined;

    QByteArray inputBuffer;
    quint32 maxLengthPacket=60000;
    void sendLogin();
    QMutex *mutexInputData;
    void parce();
    void commandHandler(QByteArray data);

    void command01(QByteArray data);
    void sendCommand01(quint8 direction ,quint16 pingCode);

    void command02 (QByteArray data); //Подтверждение получения пакета
    void sendCommand02(quint8 plugin, quint8 command, quint32 idBusiness,quint32 CRC);

    void command03 (QByteArray data); //Авторизация

    void command05 (QByteArray data); //Данные текущего пользователя
    UNAccountInfo currentAccInfo;

    QMutex *mutexSendData;

    QTimer *timerConnection;
    QTimer *timerAuthorize;
    quint8 waitTimeConnection = 20;
    quint8 waitTimeAuthorize = 20;

signals:
    void signalConnected();
    void signalAuthorized();
    void signalDisconnected();
    void signalNewData(QByteArray data);
    void signalError (UnErr err);
    void signalPingTime(quint16 time);
    void signalGetServerSettings();

    void signalNewExtPacket (quint8 plugin,quint8 command, quint16 idBusiness, QByteArray data);

    void signalUserInfoReceived();

    void signalErrorPacket(quint8 plugin,quint8 command);
    void signalConfirmPacket(quint8 plugin,quint8 command, quint32 idBusiness, quint32 currentCRC);

    void signalNewLogData(QString text);

    void signalCurrentHostIndexChanged(int index);

public slots:
    void slotGetCurrentUserInfo();
    void slotReSendData (quint8 plugin,quint8 command,quint16 idBusiness,QByteArray data);

private slots:
    void slotSocketConnected();
    void slotSocketDisconnected();
    void slotSocketError(QAbstractSocket::SocketError err);
    void slotHostFound();
    void slotSocketReadyRead();

    void slotTimerReconnect();

    void slotAuthorized();

    void slotTimerCheck();

    void slotEndSyncMissedPacket();

    void slotTimerPing();
    void slotTimerConnection();
    void slotTimerAuthorize();
};

#endif // QUNNETPROTOKOLCLIENT_H
