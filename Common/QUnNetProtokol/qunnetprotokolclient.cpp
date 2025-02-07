#include "qunnetprotokolclient.h"

QUnNetProtokolCLient::QUnNetProtokolCLient(QString programName,QObject *parent) : QObject(parent)
{
    settings = NULL;
    currentProgramName = programName;
    initClient();
}

QUnNetProtokolCLient::QUnNetProtokolCLient(QString programName,QSettings *settings_,QObject *parent) : QObject(parent)
{
    settings = settings_;
    currentProgramName = programName;
    initClient();
    loadSettings();
}

void QUnNetProtokolCLient::initClient()
{
    //Инициализация таймера пинга
    timerPing = new QTimer(this);
    connect(timerPing,SIGNAL(timeout()),this,SLOT(slotTimerPing()));
    timePing = new QElapsedTimer();

    //Инициализация таймеров подключения и авторизации
    timerConnection = new QTimer(this);
    connect(timerConnection,SIGNAL(timeout()),this,SLOT(slotTimerConnection()));
    timerAuthorize = new QTimer(this);
    connect(timerAuthorize,SIGNAL(timeout()),this,SLOT(slotTimerAuthorize()));

    mutexInputData = new QMutex;
    mutexSendData = new QMutex;
    connectionFlag = false;
    timerReconnect = NULL;
    logined = false;

    saveLogin = false;
    savePassword = false;

    crc32 = new QCrc32;
    //thrSocket = new QThread;
    socket = new QTcpSocket();

    packetManager = new QUnPacketManager(currentProgramName,false,this);
    connect(this,SIGNAL(signalConnected()),packetManager,SLOT(slotConnected()));
    connect(this,SIGNAL(signalAuthorized()),packetManager,SLOT(slotAuthorized()));
    connect(this,SIGNAL(signalDisconnected()),packetManager,SLOT(slotDisconnected()));
    connect(this,SIGNAL(signalConfirmPacket(quint8,quint8,quint32,quint32)),packetManager,SLOT(slotConfirmPacket(quint8,quint8,quint32,quint32)));
    connect(packetManager,SIGNAL(signalReSendData(quint8,quint8,quint16,QByteArray)),this,SLOT(slotReSendData(quint8,quint8,quint16,QByteArray)));
    connect(packetManager,SIGNAL(signalEndSyncMissedPacket()),this,SLOT(slotEndSyncMissedPacket()));

    //socket->moveToThread(thrSocket);
    //thrSocket->start();

    connect(socket,SIGNAL(connected()),this,SLOT(slotSocketConnected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(slotSocketDisconnected()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotSocketError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(hostFound()),this,SLOT(slotHostFound()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotSocketReadyRead()));
    connect(this,SIGNAL(signalAuthorized()),this,SLOT(slotAuthorized()));

    loadSettings();

    timerCheck = new QTimer(this);
    connect(timerCheck,SIGNAL(timeout()),this,SLOT(slotTimerCheck()));
    //timerCheck->start(1000);
}

QUnNetProtokolCLient::~QUnNetProtokolCLient()
{
    //qDebug()<<"QUnNetProtokolCLient::~QUnNetProtokolCLient start";
    saveSettings();
    //packetManager->terminate();
    packetManager->requestInterruption();
    packetManager->wait();
    delete packetManager;
    //packetManager->deleteLater();
    //thrSocket->terminate();
    socket->close();
    //delete thrSocket;
    delete socket;
    //qDebug()<<"QUnNetProtokolCLient::~QUnNetProtokolCLient end";
}

void QUnNetProtokolCLient::loadSettings()
{
    if (settings!=NULL)
    {
        currentHostNumber = settings->value("/QUnNetProtokolCLient/currentHostNumber",0).toInt();

        int countHosts = settings->value("/QUnNetProtokolCLient/countHosts",0).toInt();
        HostInfo A;
        hostsList.clear();
        for (int i=0;i<countHosts;i++)
        {
            A.address = settings->value("/QUnNetProtokolCLient/Host"+QString::number(i)).toString();
            A.port = settings->value("/QUnNetProtokolCLient/Port"+QString::number(i)).toInt();
            hostsList.push_back(A);
        }

        currentLogin = settings->value("/QUnNetProtokolCLient/login").toString();
        currentPassword = settings->value("/QUnNetProtokolCLient/password").toString();
    }
}

void QUnNetProtokolCLient::saveSettings()
{
    if (settings!=NULL)
    {
        settings->setValue("/QUnNetProtokolCLient/currentHostNumber",currentHostNumber);
        settings->setValue("/QUnNetProtokolCLient/countHosts",hostsList.length());

        for (int i=0;i<hostsList.length();i++)
        {
            settings->setValue("/QUnNetProtokolCLient/Host"+QString::number(i),hostsList.at(i).address);
            settings->setValue("/QUnNetProtokolCLient/Port"+QString::number(i),hostsList.at(i).port);
        }

        if (saveLogin) settings->setValue("/QUnNetProtokolCLient/login",currentLogin);
            else settings->setValue("/QUnNetProtokolCLient/login","");
        if (savePassword) settings->setValue("/QUnNetProtokolCLient/password",currentPassword);
            else settings->setValue("/QUnNetProtokolCLient/password","");
    }
}

bool QUnNetProtokolCLient::isConnected()
{
    return connectionFlag;
}

bool QUnNetProtokolCLient::isAuthorizes()
{
    return logined;
}

bool QUnNetProtokolCLient::connectToHost()
{
    bool Output =false;
    if ((hostsList.length()>0)&&(!connectionFlag))
    {
        Output = true;

        logined = false;
        connectionFlag = true;
        socket->connectToHost(hostsList.at(currentHostNumber).address,
                              hostsList.at(currentHostNumber).port);
        timerConnection->start(waitTimeConnection*1000);
    }

    return Output;
}

void QUnNetProtokolCLient::disconnectFromHost()
{
    if (socket->isOpen())
    {
        connectionFlag = false;
        socket->disconnectFromHost();
    }
}

QString QUnNetProtokolCLient::getLogin()
{
    return currentLogin;
}

QString QUnNetProtokolCLient::getPassword()
{
    return currentPassword;
}

UNAccountInfo QUnNetProtokolCLient::getCurrentAccountInfo()
{
    return currentAccInfo;
}

int QUnNetProtokolCLient::getMaxLengthPacket()
{
    return maxLengthPacket;
}

QVector<HostInfo> QUnNetProtokolCLient::hosts()
{
    return hostsList;
}

int QUnNetProtokolCLient::countHosts()
{
    return hostsList.length();
}

int QUnNetProtokolCLient::currentHostIndex()
{
    return currentHostNumber;
}

void QUnNetProtokolCLient::deleteHost(int index)
{
    hostsList.remove(index,1);
    saveSettings();
}

void QUnNetProtokolCLient::editHost(int index,HostInfo &host)
{
    hostsList.replace(index,host);
    saveSettings();
}

void QUnNetProtokolCLient::setHosts(QVector<HostInfo> hosts_)
{
    hostsList.clear();
    hostsList = hosts_;

    saveSettings();
}

void QUnNetProtokolCLient::setHost (HostInfo &host)
{
    hostsList.push_back(host);
    saveSettings();
}

void QUnNetProtokolCLient::setAccount (QString login,QString password)
{
    currentLogin = login;
    currentPassword = password;
    saveSettings();
}

void QUnNetProtokolCLient::setSaveLogin (bool input)
{
    saveLogin = input;
}

void QUnNetProtokolCLient::setSavePassword (bool input)
{
    savePassword = input;
}

void QUnNetProtokolCLient::setTimeConnection(quint8 time)
{
    waitTimeConnection = time;
}

void QUnNetProtokolCLient::setTimeAuthorize(quint8 time)
{
    waitTimeAuthorize = time;
}

void QUnNetProtokolCLient::slotSocketConnected()
{
    //qDebug()<<"slotSocketConnected";
    emit signalNewLogData("slotSocketConnected");
    sendLogin();
    emit signalConnected();

    if (timerConnection->isActive())
        timerConnection->stop();

    timerAuthorize->start(waitTimeAuthorize*1000);
    timerPing->start(pingInterval*1000);
}

void QUnNetProtokolCLient::slotSocketDisconnected()
{
    //qDebug()<<"slotSocketDisconnected";
    emit signalNewLogData("slotSocketDisconnected");
    logined = false;
    if (connectionFlag)
    {
        connectionFlag = false;
        QTimer::singleShot(3000,this,SLOT(slotTimerReconnect()));
    }
    emit signalDisconnected();

    if (timerPing->isActive())
                timerPing->stop();

    flagPing = false;
    countDebugPing =0;
}

void QUnNetProtokolCLient::slotTimerReconnect()
{
    currentHostNumber = getNextHostNumber(currentHostNumber);
    connectToHost();
}

void QUnNetProtokolCLient::slotSocketError(QAbstractSocket::SocketError err)
{
    //qDebug()<<"slotSocketError:"<<err;
    emit signalNewLogData("slotSocketError:" + QString::number(err));
    if (err ==  QAbstractSocket::ConnectionRefusedError)
    {
        if (timerAuthorize->isActive())
                timerAuthorize->stop();

        if (timerConnection->isActive())
                timerConnection->stop();

        connectionFlag = false;
        QTimer::singleShot(3000,this,SLOT(slotTimerReconnect()));
    }
}

void QUnNetProtokolCLient::slotHostFound()
{
    //qDebug()<<"slotHostFound";
    emit signalNewLogData("slotHostFound");
}

void QUnNetProtokolCLient::slotSocketReadyRead()
{
    mutexInputData->lock();
    inputBuffer.push_back(socket->readAll());
    parce();
}

void QUnNetProtokolCLient::sendLogin()
{
    QByteArray buffer;
    quint8 null_ = 0;
    //buffer.push_back(0x03);
    //buffer.push_back(null_);
    buffer.push_back((quint8)currentLogin.length());
    buffer.push_back(currentLogin.toUtf8());
    buffer.push_back((quint8)currentPassword.length());
    buffer.push_back(currentPassword.toUtf8());

    sendData(0x00,0x03,0x00,buffer);
}

int QUnNetProtokolCLient::getNextHostNumber (int number)\
{
    int Output = currentHostNumber + 1;
    int B = hostsList.length()-1;
    if (Output > B)
        Output = 0;

    return Output;
}

void QUnNetProtokolCLient::slotReSendData (quint8 plugin,quint8 command,quint16 idBusiness,QByteArray data)
{
    if (socket->isOpen())
    {
        quint16 currentLength = data.length()+6;
        quint8 *ptrLength = (quint8*)(&currentLength);
        quint16 countPacket = 0;
        quint8 *ptrCountPacket = (quint8*)(&countPacket);
        quint8 *ptrIdBusiness = (quint8 *)(&idBusiness);
        QByteArray buf;
        QByteArray Output;




        if (data.length()< (getMaxLengthPacket()+7))
        {
            crc32->reset();
            crc32->pushData(data);
            quint32 preCRC = crc32->getResult();

            //Добавляем к данным номер пакета
            data.insert(0,*(ptrCountPacket));
            data.insert(1,*(ptrCountPacket+1));
            data.insert(2,command);
            data.insert(3,plugin);
            data.insert(4,*ptrIdBusiness);
            data.insert(5,*(ptrIdBusiness+1));
            crc32->reset();
            crc32->pushData(data);
            quint32 currentCRC32 = crc32->getResult();
            quint8 *ptrCRC32 = (quint8*)(&currentCRC32);


            Output.push_back(0xA0);
            Output.push_back(*(ptrLength));
            Output.push_back(*(ptrLength+1));

            Output.push_back(data);

            Output.push_back(*(ptrCRC32));
            Output.push_back(*(ptrCRC32+1));
            Output.push_back(*(ptrCRC32+2));
            Output.push_back(*(ptrCRC32+3));

            socket->write(Output);
        } else
        {
            //Если все данные не помещаются в один пакет то добвим контрольную сумму всех данных
            QCrc32 *crc32_all = new QCrc32;
            crc32_all->reset();
            crc32_all->pushData(data);
            quint32 allCRC32 = crc32_all->getResult();
            quint8 *ptrAllCRC32 = (quint8*)(&allCRC32);
            data.push_back(*(ptrAllCRC32));
            data.push_back(*(ptrAllCRC32+1));
            data.push_back(*(ptrAllCRC32+2));
            data.push_back(*(ptrAllCRC32+3));


            delete crc32_all;

            countPacket = (data.length() / (getMaxLengthPacket()-20));
            if ((data.length() % (getMaxLengthPacket()-20)) >0) countPacket++;


            for (int i=0;i<data.length();i=i+getMaxLengthPacket()-20)
            {
                countPacket--;
                Output.clear();
                buf.clear();

                buf = data.mid(i,(getMaxLengthPacket()-20));

                //Добавляем к данным номер пакета
                buf.insert(0,*(ptrCountPacket));
                buf.insert(1,*(ptrCountPacket+1));

                buf.insert(2,command);
                buf.insert(3,plugin);

                buf.insert(4,*ptrIdBusiness);
                buf.insert(5,*(ptrIdBusiness+1));
                currentLength = buf.length();

                crc32->reset();
                crc32->pushData(buf);
                quint32 currentCRC32 = crc32->getResult();
                quint8 *ptrCRC32 = (quint8*)(&currentCRC32);

                Output.push_back(0xA0);
                Output.push_back(*(ptrLength));
                Output.push_back(*(ptrLength+1));

                Output.push_back(buf);

                Output.push_back(*(ptrCRC32));
                Output.push_back(*(ptrCRC32+1));
                Output.push_back(*(ptrCRC32+2));
                Output.push_back(*(ptrCRC32+3));

                socket->write(Output);
            }
        }
    }
}

void QUnNetProtokolCLient::sendData (quint8 plugin, quint8 command,quint16 idBusiness, QByteArray data, bool save)
{
    mutexSendData ->lock();
    /*
    qDebug()<<"Send data:"
            <<" command ="<<command
           <<" plugin="<<plugin
          <<" idBusiness="<<idBusiness;
          */

    if (socket->isOpen())
    {
        quint16 currentLength = data.length()+6;
        quint8 *ptrLength = (quint8*)(&currentLength);
        quint16 countPacket = 0;
        quint8 *ptrCountPacket = (quint8*)(&countPacket);
        QByteArray buf;
        QByteArray Output;
        quint8 *ptrIdBusiness = (quint8*)(&idBusiness);


        if (data.length()< (getMaxLengthPacket()+7))
        {
            crc32->reset();
            crc32->pushData(data);
            quint32 preCRC = crc32->getResult();

            PacketInfo A;
            A.plugin = plugin;
            A.command = command;
            A.idUser = currentAccInfo.id;
            A.idBusiness = idBusiness;
            A.CRC = preCRC;
            A.data = data;

            if (!((command == 0x02)&&(plugin == 0x00))) {
                packetManager->addPacket(A,save);
            }

            //Добавляем к данным номер пакета
            data.insert(0,*(ptrCountPacket));
            data.insert(1,*(ptrCountPacket+1));
            data.insert(2,command);
            data.insert(3,plugin);
            data.insert(4,*ptrIdBusiness);
            data.insert(5,*(ptrIdBusiness+1));
            crc32->reset();
            crc32->pushData(data);
            quint32 currentCRC32 = crc32->getResult();
            quint8 *ptrCRC32 = (quint8*)(&currentCRC32);


            Output.push_back(0xA0);
            Output.push_back(*(ptrLength));
            Output.push_back(*(ptrLength+1));

            Output.push_back(data);

            Output.push_back(*(ptrCRC32));
            Output.push_back(*(ptrCRC32+1));
            Output.push_back(*(ptrCRC32+2));
            Output.push_back(*(ptrCRC32+3));



            socket->write(Output);
        } else
        {
            //Если все данные не помещаются в один пакет то добвим контрольную сумму всех данных
            QCrc32 *crc32_all = new QCrc32;
            crc32_all->reset();
            crc32_all->pushData(data);
            quint32 allCRC32 = crc32_all->getResult();
            quint8 *ptrAllCRC32 = (quint8*)(&allCRC32);
            data.push_back(*(ptrAllCRC32));
            data.push_back(*(ptrAllCRC32+1));
            data.push_back(*(ptrAllCRC32+2));
            data.push_back(*(ptrAllCRC32+3));


            delete crc32_all;

            PacketInfo A;
            A.plugin = plugin;
            A.command = command;
            A.idUser = currentAccInfo.id;
            A.CRC = allCRC32;
            A.data = data;

            if (!((command == 0x02)&&(plugin == 0x00))) {
                packetManager->addPacket(A,save);
            }

            countPacket = (data.length() / (getMaxLengthPacket()-20));
            if ((data.length() % (getMaxLengthPacket()-20)) >0) countPacket++;


            for (int i=0;i<data.length();i=i+getMaxLengthPacket()-20)
            {
                countPacket--;
                Output.clear();
                buf.clear();

                buf = data.mid(i,(getMaxLengthPacket()-20));

                //Добавляем к данным номер пакета
                buf.insert(0,*(ptrCountPacket));
                buf.insert(1,*(ptrCountPacket+1));

                buf.insert(2,command);
                buf.insert(3,plugin);
                buf.insert(4,*ptrIdBusiness);
                buf.insert(5,*(ptrIdBusiness+1));
                currentLength = buf.length();

                crc32->reset();
                crc32->pushData(buf);
                quint32 currentCRC32 = crc32->getResult();
                quint8 *ptrCRC32 = (quint8*)(&currentCRC32);

                Output.push_back(0xA0);
                Output.push_back(*(ptrLength));
                Output.push_back(*(ptrLength+1));

                Output.push_back(buf);

                Output.push_back(*(ptrCRC32));
                Output.push_back(*(ptrCRC32+1));
                Output.push_back(*(ptrCRC32+2));
                Output.push_back(*(ptrCRC32+3));

                socket->write(Output);
            }
        }
    }
    mutexSendData->unlock();
}

void QUnNetProtokolCLient::sendExtData(quint8 plugin, quint8 command, QByteArray data, quint16 idBusiness, bool save)
{
    //qDebug()<<"sendExtData";
    emit signalNewLogData("sendExtData");
    QByteArray buffer;
    //buffer.push_back(command);
    //buffer.push_back(plugin);
    buffer.push_back(data);

    sendData(plugin,command,idBusiness,buffer,save);
}

void QUnNetProtokolCLient::sendCommand01(quint8 direction, quint16 pingCode)
{
    //qDebug()<<"QUnNetProtokolCLient::sendCommand01";
    emit signalNewLogData("QUnNetProtokolCLient::sendCommand01");
    QByteArray buffer;
    quint8 *ptrPingCode = (quint8*)(&pingCode);
    buffer.push_back(direction);
    buffer.push_back(*(ptrPingCode));
    buffer.push_back(*(ptrPingCode+1));

    sendData(0x00,0x01,0x00,buffer);
}

void QUnNetProtokolCLient::sendPing()
{
    //qDebug()<<"QUnNetProtokolCLient::sendPing";

    if (timePing != NULL)
            delete timePing;

    timePing = new QElapsedTimer;

    currentPingCode = (quint16)rand();
    //qDebug()<<"currentPingCode="<<QString::number(currentPingCode);
    sendCommand01(0,currentPingCode);
    flagPing = true;
    //qDebug()<<"QUnNetProtokolCLient::sendPing,currentPingCode="<<QString::number(currentPingCode);

    timePing->start();
}

void QUnNetProtokolCLient::parce()
{
    //qDebug()<<"QUnNetProtokolCLient::parce";
    int currentIndex;
    quint16 currentLength;
    quint8 *ptrLength= (quint8*)(&currentLength);
    QByteArray currentData;
    quint32 currentCRC32;
    quint8 *ptrCRC32 = (quint8*)(&currentCRC32);
    while (inputBuffer.length()>0)
    {
        currentIndex = inputBuffer.indexOf(UNheaderData);
        if (currentIndex>=0)
        {
            inputBuffer.remove(0,currentIndex);
            *(ptrLength) = inputBuffer.at(1);
            *(ptrLength+1) = inputBuffer.at(2);
            if (currentLength<=maxLengthPacket) //Проверяем длину пакета
            {
                if (inputBuffer.length()>=(currentLength+7))
                {
                    currentData = inputBuffer.mid(3,currentLength);
                    *(ptrCRC32) = inputBuffer.at(currentLength+3);
                    *(ptrCRC32+1) = inputBuffer.at(currentLength+4);
                    *(ptrCRC32+2) = inputBuffer.at(currentLength+5);
                    *(ptrCRC32+3) = inputBuffer.at(currentLength+6);

                    crc32->reset();
                    crc32->pushData(currentData);
                    if (currentCRC32 == crc32->getResult())
                    {
                        //Пакет верный
                        //qDebug()<<"QUnNetProtokolCLient::parce true packet";
                        inputBuffer.remove(0,currentLength+7);
                        emit signalNewData(currentData);
                        commandHandler(currentData);

                    } else
                    {
                        inputBuffer.remove(0,1); //Если CRC не сходитя до удаляем заголовок
                    }
                } else
                {
                    break;
                }
            } else
            {
                inputBuffer.remove(0,1); //Если длина пакета больше максимальной то удаляем заголовок
            }
        } else
        {
            break;
        }

    }
    //qDebug()<<"QUnNetProtokolCLient::parce end";
    mutexInputData->unlock();
}

bool QUnNetProtokolCLient::addPacketInList (PacketBuffer &input)
{
    //qDebug()<<"QUnNetProtokolCLient::addPacketInList "<<input.command<<" "<<input.count;
    bool Output = false;


    int currentIndex =-1;
    for (int i=0;i<packetList.length();i++)
        if (packetList.at(i).command == input.command)
        {
            currentIndex = i;
            break;
        }

    PacketBuffer A;

    if (currentIndex >=0)
    {
        A = packetList.at(currentIndex);

        if (A.count!=0)
        {
            A.buf.push_back(input.buf);
        }
        else
        {
            QByteArray buf = input.buf.mid(input.buf.length()-5,4);
            input.buf.remove(input.buf.length()-5,4);
            A.buf.push_back(input.buf);

            quint32 currentCRC32;
            quint8 *ptrCurrentCRC32 = (quint8*)(&currentCRC32);
            for (int i=0;i<buf.length();i++)
                *(ptrCurrentCRC32+i) = buf.at(i);

            QCrc32 *crc32All = new QCrc32;
            crc32All->reset();
            crc32All->pushData(A.buf);

            if (currentCRC32!=crc32All->getResult())
            {
                //qDebug()<<"Error multipacket";
                quint16 currentCommand = input.command;
                quint8 *ptrCurrentCommand = (quint8*)(&currentCommand);
                packetList.remove(currentIndex,1);

                emit signalErrorPacket(*(ptrCurrentCommand+1),*ptrCurrentCommand);
            }

        }



        A.count = input.count;
        A.dateTime = QDateTime::currentDateTime();

        packetList.replace(currentIndex,A);
    } else
    {
        packetList.push_back(input);
    }

    return Output;
}

QByteArray QUnNetProtokolCLient::getPacket(quint16 command, quint32 idBusiness)
{
    //qDebug()<<"QUnNetProtokolCLient::getPacket";
    QByteArray Output;
    PacketBuffer A;
    quint8 *ptr = (quint8*)(&A.command);

    quint8 plugin = *(ptr+1);
    quint8 currentCommand = *ptr;

    QCrc32 *crc32All = new QCrc32;


    int currentIndex =-1;
    for (int i=0;i<packetList.length();i++)
        if ((packetList.at(i).command == command) &&
                (packetList.at(i).idBusiness == idBusiness))
        {
            currentIndex = i;
            break;
        }

    if (currentIndex>=0)
    {
        A = packetList.at(currentIndex);

        crc32All->reset();
        crc32All->pushData(A.buf);
        quint32 currentCRC32 = crc32All->getResult();

        plugin = *(ptr+1);
        currentCommand = *ptr;
        if (!((plugin == 0) && (currentCommand == 0x02)))
            sendCommand02(plugin,currentCommand,A.idBusiness,currentCRC32);
        Output = A.buf;
        packetList.remove(currentIndex);

    }

    delete crc32All;

    return Output;
}

void QUnNetProtokolCLient::commandHandler(QByteArray data)
{
    //qDebug()<<"QUnNetProtokolCLient::commandHandler";
    /*
    QString debugBuf;
    for (int i=0;i<data.length();i++)
        debugBuf = debugBuf + QString::number(data.at(i),16).right(2) + " ";

    qDebug()<<"data="<<debugBuf;
    */

    quint16 command;
    quint8 *ptrCommand = (quint8*)(&command);
    quint16 countPacket;
    quint8 *ptrCountPacket = (quint8*)(&countPacket);
    quint16 idBusiness;
    quint8 *ptrIdBusiness = (quint8*)(&idBusiness);

    *ptrCountPacket = data.at(0);
    *(ptrCountPacket +1) = data.at(1);

    *ptrCommand = data.at(2);
    *(ptrCommand +1) = data.at(3);

    *ptrIdBusiness = data.at(4);
    *(ptrIdBusiness + 1) = data.at(5);

    /*
    qDebug()<<"command="<<QString::number(*((quint8*)(&command)))
            <<" plugin="<<QString::number(*((quint8*)(&command)+1))
            <<" idBusiness="<<idBusiness;
            */
    //qDebug()<<"count="<<countPacket;

    PacketBuffer A;
    if (countPacket == 0)
    {
        A.command = command;
        A.count = countPacket;
        A.idBusiness = idBusiness;
        A.buf = data.mid(6,data.length()-6);

        addPacketInList(A);

        if (*(ptrCommand+1) == 0)
        {
            switch (command) {
            case 0x0001: command01(getPacket(command,idBusiness));
                break;
            case 0x0002: command02(getPacket(command,idBusiness));
                break;
            case 0x0003: command03(getPacket(command,idBusiness));
                break;
            case 0x0005: command05(getPacket(command,idBusiness));
                break;
            }

        } else
        {
            emit signalNewExtPacket(*(ptrCommand+1),*ptrCommand,idBusiness,getPacket(command,idBusiness));
        }
    } else
    {
        //qDebug()<<"MultiPacket "<<countPacket;
        A.command = command;
        A.count = countPacket;
        A.idBusiness = idBusiness;
        A.buf = data.mid(6,data.length()-6);

        addPacketInList(A);
    }

}

void QUnNetProtokolCLient::command01(QByteArray data)
{
    //qDebug()<<"QUnNetProtokolCLient::command01";
    quint8 currentDirection = data.at(0);
    quint16 currentCode;
    quint8 *ptrCurrentCode = (quint8*)(&currentCode);
    quint16 currentTime;

    *(ptrCurrentCode) = data.at(1);
    *(ptrCurrentCode+1) = data.at(2);

    /*
    qDebug()<<"QUnNetProtokolCLient::command01, dir-"
           <<QString::number(currentDirection)<<" currentCode="
            <<QString::number(currentCode)
           <<" currentPingCode="<<QString::number(currentPingCode);
    */
    if (currentDirection == 0) //Если пинг входящий то меняем направление и возвращаем данные
    {
            sendCommand01(0x01,currentCode);
    }

   if (currentDirection == 1)
   {
       if (currentPingCode == currentCode)
       {
           currentTime = timePing->elapsed();
           //qDebug()<<"Time ping: "<<currentTime<<" ms";
           flagPing = false;
           emit signalPingTime(currentTime);
       }
   }
}

void QUnNetProtokolCLient::command02 (QByteArray data)
{
    //qDebug()<<"QUnNetProtokolCLient::command02";


    quint8 plugin = data.at(0);
    quint8 command = data.at(1);

    quint32 idBusiness;
    quint8 *ptrIdBusiness = (quint8*)(&idBusiness);

    *(ptrIdBusiness) = data.at(2);
    *(ptrIdBusiness+1) = data.at(3);
    *(ptrIdBusiness+2) = data.at(4);
    *(ptrIdBusiness+3) = data.at(5);
    data.remove(0,4);

    quint32 currentCRC;
    quint8 *ptrCRC = (quint8*)(&currentCRC);

    *(ptrCRC) = data.at(2);
    *(ptrCRC+1) = data.at(3);
    *(ptrCRC+2) = data.at(4);
    *(ptrCRC+3) = data.at(5);

    /*
    qDebug()<<"plugin="<<QString::number(plugin,16).right(2)
            <<" command="<<QString::number(command,16).right(2)
            <<" idBusiness ="<<QString::number(idBusiness,16).right(2)
            <<" CRC="<<QString::number(currentCRC,16).right(8);
            */

    emit signalConfirmPacket(plugin,command,idBusiness,currentCRC);
}

void QUnNetProtokolCLient::sendCommand02(quint8 plugin, quint8 command, quint32 idBusiness,quint32 CRC)
{
    //qDebug()<<"QUnNetProtokolCLient::sendCommand02";
    quint8 *ptrCRC = (quint8*)(&CRC);
    quint8 *ptrIdBusiness = (quint8*)(&idBusiness);

    QByteArray buffer;
    quint8 null_ =0;

    buffer.push_back(plugin);
    buffer.push_back(command);

    buffer.push_back(*(ptrIdBusiness));
    buffer.push_back(*(ptrIdBusiness+1));
    buffer.push_back(*(ptrIdBusiness+2));
    buffer.push_back(*(ptrIdBusiness+3));

    buffer.push_back(*(ptrCRC));
    buffer.push_back(*(ptrCRC+1));
    buffer.push_back(*(ptrCRC+2));
    buffer.push_back(*(ptrCRC+3));

    /*
    qDebug()<<"plugin="<<QString::number(plugin,16).right(2)<<" "
            <<"command="<<QString::number(command,16).right(2)<<" "
            <<"idBusiness="<<QString::number(idBusiness,16).right(8)<<" "
            <<"CRC="<<QString::number(CRC,16).right(8);
            */

    sendData(null_,0x02,0,buffer);
}

void QUnNetProtokolCLient::command03 (QByteArray data)
{
    //qDebug()<<"command03";

    /*
    QString debugBuf;
    for (int i=0;i<data.length();i++)
        debugBuf = debugBuf + QString::number(data.at(i),16).right(2) + " ";

    qDebug()<<"data="<<debugBuf;
    */

    quint16 code;
    quint8 *ptrCode = (quint8*)(&code);
    *(ptrCode) = data.at(1);
    *(ptrCode + 1) = data.at(2);

    if (data.at(0))
    {
        logined = true;
        if (timerAuthorize->isActive())
                timerAuthorize->stop();
        emit signalAuthorized();
    } else
    {
        connectionFlag = false;
        logined = false;
        emit signalError((UnErr)code);
    }

    //qDebug()<<"command03 end";
}

void QUnNetProtokolCLient::command05 (QByteArray data)
{
    //qDebug()<<"QUnNetProtokolCLient::command05";

    currentAccInfo.fromByteArray(data);
    //qDebug()<<currentAccInfo.toString();
    //emit signalUserInfoReceived();
    packetManager->checkMissedPacket(currentAccInfo.id);
    //qDebug()<<"QUnNetProtokolCLient::command05 end";
}

void QUnNetProtokolCLient::slotGetCurrentUserInfo()
{
    //qDebug()<<"QUnNetProtokolCLient::slotGetCurrentUserInfo";
    if (logined)
    {
        QByteArray buffer;
        quint8 null_ =0;
        //buffer.push_back(0x05);
        //buffer.push_back(null_);
        buffer.push_back(null_);

        sendData(0x00,0x05,0x00,buffer);
    }
}

void QUnNetProtokolCLient::slotAuthorized()
{
    //qDebug()<<"QUnNetProtokolCLient::slotAuthorized";
    slotGetCurrentUserInfo();
}

void QUnNetProtokolCLient::slotTimerCheck()
{
    //qDebug()<<"inputBuffer.length()="<<inputBuffer.length();
    //if (inputBuffer.length() >10) parce();
}

void QUnNetProtokolCLient::slotEndSyncMissedPacket()
{
    emit signalUserInfoReceived();
}

void QUnNetProtokolCLient::slotTimerPing()
{
    if (!flagPing)
    {
        //Если предыдущий пинг прошел то отсылаем новый
        sendPing();
    }
        else
    {
        //Если произошла ощибка пинга, то закрываем соединение
        socket->disconnectFromHost();
    }
}

void QUnNetProtokolCLient::slotTimerConnection()
{
    timerConnection->stop();

    socket->disconnectFromHost();
    socket->close();
    socket->waitForDisconnected();
    connectionFlag = false;
    QTimer::singleShot(3000,this,SLOT(slotTimerReconnect()));
}

void QUnNetProtokolCLient::slotTimerAuthorize()
{
    timerAuthorize->stop();

    socket->disconnectFromHost();
    socket->close();
    socket->waitForDisconnected();
    connectionFlag = false;
    QTimer::singleShot(3000,this,SLOT(slotTimerReconnect()));
}

bool QUnNetProtokolCLient::waitForDisconnected(int msecs)
{
   socket->waitForDisconnected();
}
