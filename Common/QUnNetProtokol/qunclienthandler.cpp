#include "qunclienthandler.h"

QUnClientHandler::QUnClientHandler(QUnAccountManager *accManager,QTcpSocket *socket, QObject *parent) : QObject(parent)
{
    Authorized = false;
    crc32 = new QCrc32(this);
    accountManager = accManager;

    /*
    packetManager = new QUnPacketManager(true);
    packetManager->slotConnected();
    packetManager->slotAuthorized();
    //connect(this,SIGNAL(signalConnected()),packetManager,SLOT(slotConnected()));
    //connect(this,SIGNAL(signalAuthorized()),packetManager,SLOT(slotAuthorized()));
    //connect(this,SIGNAL(signalDisconnected()),packetManager,SLOT(slotDisconnected()));
    connect(this,SIGNAL(signalConfirmPacket(quint8,quint8,quint32,quint32)),packetManager,SLOT(slotConfirmPacket(quint8,quint8,quint32,quint32)));
    connect(packetManager,SIGNAL(signalReSendData(quint8,quint8,quint16,QByteArray)),this,SLOT(slotReSendData(quint8,quint8,quint16,QByteArray)));
    //connect(packetManager,SIGNAL(signalEndSyncMissedPacket()),this,SLOT(slotEndSyncMissedPacket()));
    connect(packetManager,SIGNAL(signalDebugMessage(QString)),this,SIGNAL(signalDebugMessage(QString)));
    */

    mutexSendData = new QMutex;

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    client = socket;
    connect(client,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    connect(client,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));


    timerCheckPacket = new QTimer(this);
    connect(timerCheckPacket,SIGNAL(timeout()),this,SLOT(slotUpdateTimerCheckPacket()));
    timerCheckPacket->start(1000);

    //Инициализация таймера пинга
    timerPing = new QTimer(this);
    connect(timerPing,SIGNAL(timeout()),this,SLOT(slotTimerPing()));
    timerPing->start(pingInterval*1000);
}

QUnClientHandler::~QUnClientHandler()
{
    /*
    packetManager->slotDisconnected();
    //packetManager->terminate();
    //packetManager->wait();
    packetManager->deleteLater();
    */
    emit signalDebugMessage("User " + currentAccount.login + " disconnected");
    //qDebug()<<"User "<<currentAccount.login<<" disconnected";
    delete mutexSendData;
}

UNAccountInfo *QUnClientHandler::getCurrentAccountInfo()
{
    return &currentAccount;
}

void QUnClientHandler::parce()
{
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

}

void QUnClientHandler::disconnectCLient()
{
    emit signalDebugMessage("Client disconnect");
    //qDebug()<<"QUnClientHandler::disconnectCLient";
    client->close();
}

void QUnClientHandler::slotDisconnected()
{
    //qDebug()<<"QUnClientHandler::slotDisconnected";

    emit signalDisconnected();
    deleteLater();
}

void QUnClientHandler::slotReadyRead()
{
    inputBuffer.push_back(client->readAll());
    parce();
}

bool QUnClientHandler::addPacketInList (PacketBuffer_ &input)
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

    PacketBuffer_ A;

    if (currentIndex >=0)
    {
        A = packetList.at(currentIndex);

        if (A.count - input.count >1)
        {
            //qDebug()<<"Bad packet A.buf.clear()";
            A.buf.clear();
        }

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

                //qDebug()<<"Error packet";
                emit signalErrorPacket(*(ptrCurrentCommand+1),*ptrCurrentCommand);
            }
            else
            {
                //Есди мультипакет верный то топравляем подтверждение

            }
        }

        A.count = input.count;
        A.dateTime = QDateTime::currentDateTime();

        packetList.replace(currentIndex,A);
    } else
    {
        input.dateTime = QDateTime::currentDateTime();
        packetList.push_back(input);
    }

    return Output;
}

QByteArray QUnClientHandler::getPacket(quint16 command)
{
    //qDebug()<<"QUnNetProtokolCLient::getPacket";
    QByteArray Output;

    int currentIndex =-1;
    for (int i=0;i<packetList.length();i++)
        if (packetList.at(i).command == command)
        {
            currentIndex = i;
            break;
        }

    if (currentIndex>=0)
    {
        PacketBuffer_ A = packetList.at(currentIndex);
        quint8 *ptr = (quint8*)(&A.command);

        quint8 plugin = *(ptr+1);
        quint8 command = *ptr;

        QCrc32 *crc32All = new QCrc32;
        crc32All->reset();
        crc32All->pushData(A.buf);
        quint32 currentCRC32 = crc32All->getResult();

        delete crc32All;

        if (!((plugin ==0) && (command == 0x02)))
            sendCommand02(plugin,command,A.idBusiness,currentCRC32);

        Output = A.buf;
        packetList.remove(currentIndex);
    }

    return Output;
}

void QUnClientHandler::commandHandler(QByteArray data)
{
    //qDebug()<<"QUnClientHandler::commandHandler";
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

    emit signalDebugMessage("User "+ currentAccount.login +
                            " idBusiness ="+QString::number(idBusiness) +
                            " plugin="+QString::number(*(ptrCommand+1)) +
                            " command=" + QString::number(*ptrCommand,16).right(2));
    /*
    qDebug()<<"User "<<currentAccount.login
            <<" idBusiness ="<<idBusiness
            <<" plugin="<<*(ptrCommand+1)
            <<" command="<<QString::number(*ptrCommand,16).right(2);
            */

    emit signalDebugMessage("Received command: "+QString::number(command,16).right(4));

    PacketBuffer_ A;
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
            case 0x0001: command01(getPacket(command));
                break;
            case 0x0002: command02(getPacket(command));
                break;
            case 0x0003: command03(getPacket(command));
                break;
            case 0x0005: command05(getPacket(command));
                break;
            }

        } else
        {
            emit signalNewExtData(*(ptrCommand+1),*ptrCommand,idBusiness,getPacket(command),this);
        }
    } else
    {
        A.command = command;
        A.count = countPacket;
        A.idBusiness = idBusiness;
        A.buf = data.mid(6,data.length()-6);

        addPacketInList(A);
    }
}

void QUnClientHandler::command01(QByteArray data)
{
    //qDebug()<<"QUnClientHandler::command01";
    emit signalDebugMessage("QUnClientHandler::command01");

    quint8 currentDirection = data.at(0);
    quint16 currentCode;
    quint8 *ptrCurrentCode = (quint8*)(&currentCode);
    quint16 currentTime;

    *(ptrCurrentCode) = data.at(1);
    *(ptrCurrentCode+1) = data.at(2);

    //qDebug()<<"Direction:"<<QString::number(currentDirection)<<" current code:"<<QString::number(currentCode);
    emit signalDebugMessage("Direction:" + QString::number(currentDirection) +
                            " current code:" + QString::number(currentCode));
    /*
    qDebug()<<"QUnClientHandler::command01, dir-"
           <<QString::number(currentDirection)<<" currentCode="
            <<QString::number(currentCode);
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
           flagPing = false;
           emit signalPingTime(currentTime);
       }
   }


}

void QUnClientHandler::command03 (QByteArray data)
{
    //qDebug()<<"QUnClientHandler::command03";
    emit signalDebugMessage("QUnClientHandler::command03");

    QString login;
    QString password;
    quint8 loginLenght = data.at(0);
    login = QString(data.mid(1,loginLenght));
    quint8 passwordLength = data.at(loginLenght+1);
    password = QString(data.mid(loginLenght+2,passwordLength));
    //qDebug()<<"Login="<<login;
    //qDebug()<<"Password="<<password;
    emit signalDebugMessage("User authorization, login: "+login+" Password: "+password);

    if (accountManager->checkLogin(login))
    {
        //qDebug()<<"Login is correct";
        int id = accountManager->indexOf(login);
        if (accountManager->checkPassword(id,password))
        {
            //qDebug()<<"Password is correct";
            if (accountManager->isEnable(id))
            {
                Authorized = true;
                currentAccount = accountManager->getAccountInfo(login);
                sendCommand03(true,0);
                emit signalDebugMessage("User authorization...OK");
                emit signalReceiveAccountInfo(this);
            } else
            {
                emit signalDebugMessage("User authorization...ERROR errAuthBlock");
                sendCommand03(false,UnErr::errAuthBlock);
                disconnectCLient();
            }
        } else
        {
            emit signalDebugMessage("User authorization...ERROR errAuthPassword");
            sendCommand03(false,UnErr::errAuthPassword);
            disconnectCLient();
        }

    } else
    {
        emit signalDebugMessage("User authorization...ERROR errAuthLogin");
        sendCommand03(false,UnErr::errAuthLogin);
        disconnectCLient();
    }

}

void QUnClientHandler::sendData (quint8 plugin, quint8 command,quint16 idBusiness, QByteArray data, bool sync)
{

    emit signalDebugMessage("Send data: user=" + currentAccount.login +
                            " command =" + QString::number(command,16).right(2) +
                            " plugin=" + QString::number(plugin,16).right(2) +
                            " idBusiness=" + QString::number(idBusiness,16).right(4));
    /*
    qDebug()<<"Send data:"
            <<" user="<<currentAccount.login
            <<" command ="<<command
            <<" plugin="<<plugin
            <<" idBusiness="<<idBusiness;
            */

    if (client->isOpen())
    {
        emit signalDebugMessage("Sending data to the user (sendData)");
        quint16 currentLength = data.length()+6;
        quint8 *ptrLength = (quint8*)(&currentLength);
        quint16 countPacket = 0;
        quint8 *ptrCountPacket = (quint8*)(&countPacket);
        quint8 *ptrIdBusiness = (quint8*)(&idBusiness);
        QByteArray buf;
        QByteArray Output;


        if (data.length()< (getMaxLengthPacket()+7))
        {
            crc32->reset();
            crc32->pushData(data);
            quint32 preCRC = crc32->getResult();

            PacketInfo A;
            A.plugin = plugin;
            A.command = command;
            A.idUser = currentAccount.id;
            A.idBusiness = idBusiness;
            A.CRC = preCRC;
            A.data = data;

            /*
            if (!((command == 0x02)&&(plugin == 0x00))) {
                packetManager->addPacket(A);
            }
            */

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

            client->write(Output);
        } else
        {
            //Если все данные не помещаются в один пакет то добвим контрольную сумму всех данных
            QCrc32 *crc32_all = new QCrc32;
            crc32_all->reset();
            crc32_all->pushData(data);
            quint32 allCRC32 = crc32_all->getResult();
            quint8 *ptrAllCRC32 = (quint8*)(&allCRC32);


            delete crc32_all;

            data.push_back(*(ptrAllCRC32));
            data.push_back(*(ptrAllCRC32+1));
            data.push_back(*(ptrAllCRC32+2));
            data.push_back(*(ptrAllCRC32+3));

            PacketInfo A;
            A.plugin = plugin;
            A.command = command;
            A.idUser = currentAccount.id;
            A.idBusiness = idBusiness;
            A.CRC = allCRC32;
            A.data = data;



            /*
            if (!((command == 0x02)&&(plugin == 0x00))) {
                packetManager->addPacket(A);
            }
            */

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

                client->write(Output);

                //qDebug()<<"countPacket="<<countPacket;
                emit signalDebugMessage("countPacket=" + QString::number(countPacket));
            }
        }        
    }

}

void QUnClientHandler::sendCommand01(quint8 direction, quint16 pingCode)
{
    //qDebug()<<"QUnClientHandler::sendCommand01";
    QByteArray buffer;
    quint8 null_ =0;
    quint8 *ptrPingCode = (quint8*)(&pingCode);

    buffer.push_back(direction);
    buffer.push_back(*(ptrPingCode));
    buffer.push_back(*(ptrPingCode+1));

    sendData(null_,0x01,0,buffer);
    emit signalDebugMessage("sendCommand01");
}

void QUnClientHandler::sendPing()
{
    if (timePing != NULL) delete timePing;
    timePing = new QTime;

    currentPingCode = (quint16)qrand();
    sendCommand01(0,currentPingCode);
    flagPing = true;
    timePing->start();
}

void QUnClientHandler::command02(QByteArray data)
{
    //qDebug()<<"QUnClientHandler::command02";
    emit signalDebugMessage("QUnClientHandler::command02");

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

    emit signalDebugMessage("plugin=" + QString::number(plugin,16).right(2) +
                            " command=" + QString::number(command,16).right(2) +
                            " idBusiness =" + QString::number(idBusiness,16).right(2) +
                            " CRC=" + QString::number(currentCRC,16).right(8));
    /*
    qDebug()<<"plugin="<<QString::number(plugin,16).right(2)
            <<" command="<<QString::number(command,16).right(2)
            <<" idBusiness ="<<QString::number(idBusiness,16).right(2)
            <<" CRC="<<QString::number(currentCRC,16).right(8);
            */

    emit signalConfirmPacket(plugin,command,idBusiness,currentCRC);
}

void QUnClientHandler::sendCommand02(quint8 plugin, quint8 command, quint32 idBusiness, quint32 CRC)
{
    //qDebug()<<"QUnClientHandler::sendCommand02";
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

    sendData(null_,0x02,0,buffer);
    emit signalDebugMessage("sendCommand02");
}

void QUnClientHandler::sendCommand03 (bool authFlag,quint16 addCode)
{
    //qDebug()<<"QUnClientHandler::sendCommand03";
    QByteArray buffer;
    quint8 null_ =0;
    quint8 *ptrAddCode = (quint8*)(&addCode);
    //buffer.push_back(0x03);
    //buffer.push_back(null_);
    buffer.push_back((quint8)authFlag);
    buffer.push_back(*(ptrAddCode));
    buffer.push_back(*(ptrAddCode+1));

    sendData(0x00,0x03,0,buffer);
    emit signalDebugMessage("sendCommand03");

    //qDebug()<<"QUnClientHandler::sendCommand03 end";
}

void QUnClientHandler::sendCommand04 (quint16 command,UnErr err)
{
    //qDebug()<<"QUnClientHandler::sendCommand04";
    QByteArray buffer;
    quint8 null_ =0;
    quint8 *ptrCommand = (quint8*)(&command);
    //buffer.push_back(0x04);
    //buffer.push_back(null_);
    buffer.push_back(*(ptrCommand));
    buffer.push_back(*(ptrCommand+1));
    buffer.push_back((quint8)err);

    sendData(0x00,0x04,0,buffer);
    emit signalDebugMessage("sendCommand04");

    //qDebug()<<"QUnClientHandler::sendCommand04 end";
}

void QUnClientHandler::command05 (QByteArray data)
{
    //qDebug()<<"QUnClientHandler::command05";
    emit signalDebugMessage("command05");
    if (Authorized)
    {
        sendCommand05(currentAccount);
    } else sendCommand04(0x0005,UnErr::errNoAuthorized);

    //qDebug()<<"QUnClientHandler::command05 end";
}

void QUnClientHandler::sendCommand05 (UNAccountInfo userInfo)
{
    //qDebug()<<"QUnClientHandler::sendCommand05";
    QByteArray buffer;
    quint8 null_ =0;
    //buffer.push_back(0x05);
    //buffer.push_back(null_);
    buffer.push_back(userInfo.toByteArray());

    sendData(0x00,0x05,0,buffer);
    emit signalDebugMessage("sendCommand05");
}

void QUnClientHandler::sendExtData(quint8 plugin, quint8 command,quint16 idBusiness, QByteArray data, bool sync)
{
    mutexSendData->lock();
    QByteArray buffer;
    //buffer.push_back(command);
    //buffer.push_back(plugin);
    buffer.push_back(data);

    sendData(plugin,command,idBusiness,buffer,sync);
    mutexSendData->unlock();
}

void QUnClientHandler::slotReSendData (quint8 plugin,quint8 command,quint16 idBusiness,QByteArray data)
{
    emit signalDebugMessage("Resend data: user=" + currentAccount.login +
                            " command =" + QString::number(command,16).right(2) +
                            " plugin=" + QString::number(plugin,16).right(2) +
                            " idBusiness=" + QString::number(idBusiness));

    /*
    qDebug()<<"Resend data:"
            <<" user="<<currentAccount.login
            <<" command ="<<command
            <<" plugin="<<plugin
            <<" idBusiness="<<idBusiness;
            */

    if (client->isOpen())
    {
        emit signalDebugMessage("Sending data to the user (sendData)");
        quint16 currentLength = data.length()+6;
        quint8 *ptrLength = (quint8*)(&currentLength);
        quint16 countPacket = 0;
        quint8 *ptrCountPacket = (quint8*)(&countPacket);
        quint8 *ptrIdBusiness = (quint8*)(&idBusiness);
        QByteArray buf;
        QByteArray Output;


        if (data.length()< (getMaxLengthPacket()+7))
        {
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

            client->write(Output);
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

                client->write(Output);

                emit signalDebugMessage("countPacket=" + QString::number(countPacket));
                //qDebug()<<"countPacket="<<countPacket;
            }
        }
    }
}

int QUnClientHandler::getMaxLengthPacket()
{
    return maxLengthPacket;
}

QUnPacketManager *QUnClientHandler::getPacketManager()
{
    return packetManager;
}

UNAccountInfo *QUnClientHandler::getUnAccountInfo()
{
    return &currentAccount;
}

QString QUnClientHandler::byteArrayToString (QByteArray input)
{
    QString Output;
    quint8 currentByte;

    for (int i=0;i<input.length();i++)
    {
        currentByte = input.at(i);
        if (currentByte>0x0f)
            Output = Output + QString::number(input.at(i),16).right(2)+ " ";
        else Output = Output + "0" + QString::number(input.at(i),16).right(2)+ " ";
    }

    return Output;
}

void QUnClientHandler::slotUpdateTimerCheckPacket()
{
    for (int i=0;i<packetList.length();i++)
        if (QDateTime::currentDateTime().toTime_t() - packetList.at(i).dateTime.toTime_t() >60)
        {
            //qDebug()<<"Remove old packet";
            packetList.remove(i);
            i--;
        }
}

void QUnClientHandler::slotTimerPing()
{
    if (!flagPing)
    {
        //Если предыдущий пинг прошел то отсылаем новый
        sendPing();
    }
        else
    {
        //Если произошла ощибка пинга, то закрываем соединение
        //qDebug()<<"Ping error";
        client->disconnectFromHost();
        //socket->disconnectFromHost();
    }
}
