#include "qsentolane.h"

QSentoLanE::QSentoLanE(QIODevice *device, QObject *parent) : QObject(parent)
{
    currentDevice = device;
    connect(currentDevice,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));

    timerACK = new QTimer(this);

}
QSentoLanE::~QSentoLanE()
{

}

bool QSentoLanE::sendData(QByteArray data)
{
    bool Output = true;

    if (!flagCommand)
    {
        currentDevice->write(data);
        flagCommand = true;
        emit signalTxData(data);
    }
    else commandLine.push_back(data);

    return Output;
}

void QSentoLanE::eventACK()
{
    if (commandLine.length()>0)
    {
        currentDevice->write(commandLine.at(0));
        emit signalTxData(commandLine.at(0));
        commandLine.remove(0,1);

    } else
    {
        flagCommand = false;
    }
}

void QSentoLanE::eventENQ()
{
    flagReceive = true;

    sendACK();
}

void QSentoLanE::eventEOT()
{
    flagReceive = false;
    eventACK();
}

void QSentoLanE::eventRVI()
{
    //sendACK();
    sendEOT();
}

void QSentoLanE::sendACK()
{
    QByteArray buf;
    buf.push_back(cbyteACK);
    currentDevice->write(buf);
    emit signalTxData(buf);
}

void QSentoLanE::sendEOT()
{
    QByteArray buf;
    buf.push_back(cbyteEOT);
    currentDevice->write(buf);
    emit signalTxData(buf);
}

bool QSentoLanE::addOperator (operatorCM user)
{
    bool Output = true;

    QString comm;
    QString name; //имя оператора
    QString password; //пароль оператора
    QByteArray buf;
    QString CRC;


    if (currentDevice->isOpen())
    {
        comm = "DD#001";
        if (user.number<10)
            comm = comm + "0" + QString::number(user.number);
                else comm = comm + QString::number(user.number);

        //Проверяем длину имени, если она больше 20 символов, то обрезаем ее
        if (user.name.length()>20)
            name  = user.name.mid(0,20);
                else name = user.name;

        //Если имя меньше 20 символов, то заполняем остальные символы пробелами
        while (name.length()<20)
            name = name + ' ';

        comm = comm + name;

        //Получаем пароль и преобразовываем его в строку
        password = QString::number(user.pasword);
        //Если длина пароля больше 8 символов, то обрезаем ее
        if (password.length()>8)
            password = password.mid(0,8);

        //Если пароль короче 8 символов, то заполняем недостащие символы пробелами
        while (password.length()<8)
            password = password + ' ';

        comm = comm + password;

        //Добавляем права пользователя
        if (user.right<10)
            comm = comm + "0" + QString::number(user.right);
                else comm = comm + QString::number(user.right);

        buf.clear();
        buf.push_back(toCP1251(&comm));

        for (quint8 i=0;i<16;i++)
            buf.push_back(0x30);

        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);

    } else Output = false;


    return Output;
}

bool QSentoLanE::delOperator (operatorCM user)
{
    bool Output = true;

    QString comm;
    QString name; //имя оператора
    QString password; //пароль оператора
    QByteArray buf;
    QString CRC;


    if (currentDevice->isOpen())
    {
        comm = "DD#000";
        if (user.number<10)
            comm = comm + "0" + QString::number(user.number);
                else comm = comm + QString::number(user.number);

        //Проверяем длину имени, если она больше 20 символов, то обрезаем ее
        if (user.name.length()>20)
            name  = user.name.mid(0,20);
                else name = user.name;

        //Если имя меньше 20 символов, то заполняем остальные символы пробелами
        while (name.length()<20)
            name = name + ' ';

        comm = comm + name;

        //Получаем пароль и преобразовываем его в строку
        password = QString::number(user.pasword);
        //Если длина пароля больше 8 символов, то обрезаем ее
        if (password.length()>8)
            password = password.mid(0,8);

        //Если пароль короче 8 символов, то заполняем недостащие символы пробелами
        while (password<8)
            password = password + ' ';

        comm = comm + password;

        //Добавляем права пользователя
        if (user.right<10)
            comm = comm + "0" + QString::number(user.right);
                else comm = comm + QString::number(user.right);

        buf.clear();
        buf.push_back(toCP1251(&comm));

        for (quint8 i=0;i<16;i++)
            buf.push_back(0x30);

        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);

    } else Output = false;


    return Output;
}

bool QSentoLanE::readOperators ()
{
    bool Output = true;


    if (currentDevice->isOpen())
    {
        listOperators.clear();
        startExtandedPacket packetInfo;
        packetInfo.taskType = 'D';
        packetInfo.subTask = 0x00;
        packetInfo.transType = transmissionType::ttypeCMtoPC;

        Output = sendStartExtendedPacket(packetInfo);

        QByteArray buf;
        buf.push_back(cbyteEOT);
        sendData(buf);

    } else Output = false;

    return Output;
}

bool QSentoLanE::sendStartPacket (startPacket packetInfo)
{
    bool Output = true;

    QString comm;
    QByteArray buf;
    QString CRC;

    if (currentDevice->isOpen())
    {
        comm = "S";
        comm = comm + packetInfo.taskType;

        if (packetInfo.transType<10)
            comm = comm + "0" + QString::number(packetInfo.transType);
                else  comm = comm + QString::number(packetInfo.transType);


        if (packetInfo.startNum.length()>18)
            packetInfo.startNum = packetInfo.startNum.mid(0,18);

        while (packetInfo.startNum.length()<18)
            packetInfo.startNum = packetInfo.startNum + ' ';

        comm = comm + packetInfo.startNum;

        if (packetInfo.endNum.length()>18)
            packetInfo.endNum = packetInfo.endNum.mid(0,18);

        while (packetInfo.endNum.length()<18)
            packetInfo.endNum = packetInfo.endNum + ' ';

        comm = comm + packetInfo.endNum;

        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);
    } else Output = false;

    return Output;

}

bool QSentoLanE::sendStartExtendedPacket (startExtandedPacket packetInfo)
{
    bool Output = true;

    QString comm;
    QByteArray buf;
    QString CRC;

    if ((currentDevice->isOpen()) && (packetInfo.subTask < 100))
    {
        comm = "S";
        comm = comm + packetInfo.taskType;

        comm = comm + '#';
        if (packetInfo.subTask < 10)
            comm = comm + "0" + QString::number(packetInfo.subTask);
                else comm = comm + QString::number(packetInfo.subTask);

        if (packetInfo.transType<10)
            comm = comm + "0" + QString::number(packetInfo.transType);
                else  comm = comm + QString::number(packetInfo.transType);


        if (packetInfo.startNum.length()>18)
            packetInfo.startNum = packetInfo.startNum.mid(0,18);

        while (packetInfo.startNum.length()<18)
            packetInfo.startNum = packetInfo.startNum + ' ';

        comm = comm + packetInfo.startNum;

        if (packetInfo.endNum.length()>18)
            packetInfo.endNum = packetInfo.endNum.mid(0,18);

        while (packetInfo.endNum.length()<18)
            packetInfo.endNum = packetInfo.endNum + ' ';

        comm = comm + packetInfo.endNum;

        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);

    } else Output = false;

    return Output;
}

bool QSentoLanE::openingShift ()
{
    bool Output = true;

    QString comm;
    QByteArray buf;
    QString CRC;

    if (currentDevice->isOpen())
    {
        comm = "D*C0";
        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);

    } else Output = false;

    return Output;
}

bool QSentoLanE::depositCash (quint32 whole, quint8 fract)
{
    bool Output = true;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString CRC;

        comm = comm + "D*C1";

        quint32 sum = whole*100 + fract;
        comm = comm + QString::number(sum);
        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);

    } else Output = false;

    return Output;
}

bool QSentoLanE::payCash (quint32 whole, quint8 fract)
{
    bool Output = true;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString CRC;

        comm = comm + "D*C2";

        quint32 sum = whole*100 + fract;
        comm = comm + QString::number(sum);
        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);

    } else Output = false;

    return Output;
}

bool QSentoLanE::printReport(typeReport type)
{
    bool Output = true;

    if (currentDevice->isOpen())
    {
        QByteArray buf;
        QString A;
        QString CRC;

        switch (type) {
        case typeRZConfirm: A = "D*R0";
            break;
        case typeRZNoConfirm: A = "D*R1";
            break;
        case typeRXNoConfirm: A = "D*R2";
            break;
        default: Output = false;
            break;
        }

        if (Output)
        {
            buf.push_back(A.toLatin1());
            buf.push_back(cbyteETX);
            CRC = QString::number(CalcCrc(&buf),16).right(4);
            while (CRC.length()<4) CRC.insert(0,'0');
            buf.push_back(CRC.toLatin1());

            buf.insert(0,cbyteSTX);
            sendData(buf);
        }


    } else Output = false;

    return Output;
}

bool QSentoLanE::printNonTaxDo(QStringList *text)
{
    bool Output = true;

    printLine(0,"");
    for (int i=0;i<text->length();i++)
        printLine(1,text->at(i));
    printLine(6,"");

    return Output;
}

bool QSentoLanE::printNonTaxText(QString text)
{
    bool Output = false;

    QString comm;
    QByteArray buf;
    QString CRC;
    QString currentText;

    if (currentDevice->isOpen())
    {
        while (text.length()>0)
        {
            comm.clear();
            Output = true;
            comm = "D*E";

            currentText = text.mid(0,33);
            text.remove(0,33);

            while (currentText.length()<18)
                currentText.push_back(' ');

            comm = comm + currentText;

            for (quint8 i=0;i<42;i++)
                comm.push_back(' ');

            buf.clear();
            buf.push_back(toCP1251(&comm));
            buf.push_back(cbyteETX);
            CRC = QString::number(CalcCrc(&buf),16).right(4);
            while (CRC.length()<4) CRC.insert(0,'0');
            buf.push_back(CRC.toLatin1());
            buf.insert(0,cbyteSTX);
            sendData(buf);
        }

    } else Output = false;

    return Output;
}

bool QSentoLanE::addDiscountSurcharge(discountSurchargeInfo info)
{
    bool Output = false;

    if (currentDevice->isOpen())
    {
        Output = true;
        QString comm;
        QByteArray buf;
        QString strValueWhole;
        QString strValueFract;

        QString CRC;

        comm = "D*D";
        comm = comm + info.operation;

        strValueWhole = QString::number(info.valueWhole);
        if (strValueWhole.length()>8)
            strValueWhole = strValueWhole.mid(0,8);

        while (strValueWhole.length()<8)
            strValueWhole.insert(0,' ');

        comm = comm + strValueWhole;



        strValueFract = QString::number(info.valueFract);

        if (strValueFract.length()>2)
            strValueFract = strValueFract.mid(0,2);

        if (strValueFract.length()<2)
            strValueFract.insert(0,'0');

        comm = comm + strValueFract;

        for (quint8 i=0;i<50;i++)
            comm.push_back(' ');

        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);

        sendData(buf);
    }

    return Output;
}

bool QSentoLanE::regProduct(productPosition product)
{
    bool Output = true;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString code;
        QString price;
        QString quantity;
        QString sum;
        QString name;
        QString CRC;


        comm = comm +  "D*5";
        code = QString::number(product.code);
        if (code.length()>18) code = code.mid(0,18);
        while (code.length()<18)
            code.push_back(' ');

        comm = comm + code;
        code.clear();
        comm = comm + product.vat;

        if (product.priceFract<10)
            price = price + "0" + QString::number(product.priceFract);
                else price = price +  QString::number(product.priceFract);

        price = QString::number(product.priceWhole) + price;
        if (price.length()>10) price = price.mid(0,10);
        while (price.length()<10)
            price.insert(0,' ');

        comm = comm + price;
        price.clear();



        if (product.quantityFract<10)
            quantity = quantity + "00" + QString::number(product.quantityFract);
                else if (product.priceFract<100) quantity = quantity + "0" + QString::number(product.quantityFract);
                    else quantity = quantity + QString::number(product.quantityFract);

        quantity = QString::number(product.quantityWhole) + quantity;
        if (quantity.length()>10) quantity = quantity.mid(0,10);
        while (quantity.length()<10)
            quantity.insert(0,' ');

        comm = comm + quantity;
        quantity.clear();


        if (product.sumFract<10)
            sum = sum + "0" + QString::number(product.sumFract);
                else sum = sum +  QString::number(product.sumFract);

        sum = QString::number(product.sumWhole) + sum;
        if (sum.length()>10) sum = sum.mid(0,10);
        while (sum.length()<10)
            sum.insert(0,' ');

        comm = comm + sum;
        sum.clear();


        for (quint8 i=0;i<16;i++)
            comm.push_back(' ');

        name = product.name;
        if (name.length()>35) name = name.mid(0,35);
        while (name.length()<35)
            name.insert(0,' ');

        comm = comm + name;
        name.clear();

        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);

        sendData(buf);
        qDebug()<<"buf.length="<<buf.length();

    } else Output = false;

    return Output;
}

bool QSentoLanE::revProduct (productPosition product)
{
    bool Output = true;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString code;
        QString price;
        QString quantity;
        QString sum;
        QString name;
        QString CRC;


        comm = comm +  "D*2";
        code = QString::number(product.code);
        if (code.length()>18) code = code.mid(0,18);
        while (code.length()<18)
            code.push_back(' ');

        comm = comm + code;
        code.clear();
        comm = comm + product.vat;

        if (product.priceFract<10)
            price = price + "0" + QString::number(product.priceFract);
                else price = price +  QString::number(product.priceFract);

        price = QString::number(product.priceWhole) + price;
        if (price.length()>10) price = price.mid(0,10);
        while (price.length()<10)
            price.insert(0,' ');

        comm = comm + price;
        price.clear();



        if (product.quantityFract<10)
            quantity = quantity + "00" + QString::number(product.quantityFract);
                else if (product.priceFract<100) quantity = quantity + "0" + QString::number(product.quantityFract);
                    else quantity = quantity + QString::number(product.quantityFract);

        quantity = QString::number(product.quantityWhole) + quantity;
        //quantity = "-" + quantity;
        if (quantity.length()>10) quantity = quantity.mid(0,10);
        while (quantity.length()<10)
            quantity.insert(0,' ');

        comm = comm + quantity;
        quantity.clear();


        if (product.sumFract<10)
            sum = sum + "0" + QString::number(product.sumFract);
                else sum = sum +  QString::number(product.sumFract);

        sum = QString::number(product.sumWhole) + sum;
        if (sum.length()>10) sum = sum.mid(0,10);
        while (sum.length()<10)
            sum.insert(0,' ');

        comm = comm + sum;
        sum.clear();


        for (quint8 i=0;i<16;i++)
            comm.push_back(' ');

        name = product.name;
        if (name.length()>35) name = name.mid(0,35);
        while (name.length()<35)
            name.insert(0,' ');

        comm = comm + name;
        name.clear();

        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);

        sendData(buf);
        qDebug()<<"buf.length="<<buf.length();

    } else Output = false;

    return Output;
}

bool QSentoLanE::cancelDocument()
{
    bool Output = true;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString CRC;

        comm = comm + "D*3";
        for (quint8 i =0;i<60;i++)
            comm.push_back(' ');


        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);

        sendData(buf);
    } else Output = false;


    return Output;
}

bool QSentoLanE::closeDocument(payment pay)
{
    bool Output = true;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString value;
        QString cash;
        QString check;
        QString card;
        QString coupon;
        QString credit;
        QString CRC;

        comm = comm + "D*4";

        if (pay.valueFract<10)
            value = value + "0" + QString::number(pay.valueFract);
                else value = value +  QString::number(pay.valueFract);
        value = QString::number(pay.valueWhole) + value;
        if (value.length()>10) value = value.mid(0,10);
        while (value.length()<10)
            value.insert(0,' ');
        comm = comm + value;

        if (pay.cashFract<10)
            cash = cash + "0" + QString::number(pay.cashFract);
                else cash = cash +  QString::number(pay.cashFract);
        cash = QString::number(pay.cashWhole) + cash;
        if (cash.length()>10) cash = cash.mid(0,10);
        while (cash.length()<10)
            cash.insert(0,' ');
        comm = comm + cash;

        if (pay.checkFract<10)
            check = check + "0" + QString::number(pay.checkFract);
                else check = check +  QString::number(pay.checkFract);
        check = QString::number(pay.checkWhole) + check;
        if (check.length()>10) check = check.mid(0,10);
        while (check.length()<10)
            check.insert(0,' ');
        comm = comm + check;

        if (pay.cardFract<10)
            card = card + "0" + QString::number(pay.cardFract);
                else card = card +  QString::number(pay.cardFract);
        card = QString::number(pay.cardWhole) + card;
        if (card.length()>10) card = card.mid(0,10);
        while (card.length()<10)
            card.insert(0,' ');
        comm = comm + card;

        if (pay.couponFract<10)
            coupon = coupon + "0" + QString::number(pay.couponFract);
                else coupon = coupon +  QString::number(pay.couponFract);
        coupon = QString::number(pay.couponWhole) + coupon;
        if (coupon.length()>10) coupon = coupon.mid(0,10);
        while (coupon.length()<10)
            coupon.insert(0,' ');
        comm = comm + coupon;

        if (pay.creditract<10)
            credit = credit + "0" + QString::number(pay.creditract);
                else credit = credit +  QString::number(pay.creditract);
        credit = QString::number(pay.creditWhole) + credit;
        if (credit.length()>10) credit = credit.mid(0,10);
        while (credit.length()<10)
            credit.insert(0,' ');
        comm = comm + credit;

        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);

        sendData(buf);

    } else Output = false;

    return Output;
}

bool QSentoLanE::requestDocStat()
{
    bool Output = false;

    startPacket packetInfo;
    packetInfo.taskType = '*';
    packetInfo.transType = ttypeCMtoPC;

    Output = sendStartPacket(packetInfo);
    if (Output)
        sendEOT();

    return Output;
}

bool QSentoLanE::openReturnDoc(returnDoc infoRetDoc)
{
    bool Output = false;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString strPLUCode;
        QString strQuantityWhole;
        QString strQuantityFract;

        QString CRC;

        comm = "D*A";

        strPLUCode = QString::number(infoRetDoc.code);
        if (strPLUCode.length()>18)
            strPLUCode = strPLUCode.mid(0,18);
        while (strPLUCode.length()<18)
            strPLUCode.insert(0,' ');
        comm = comm + strPLUCode;

        strQuantityWhole = QString::number(infoRetDoc.quantityWhole);
        if (strQuantityWhole.length()>7)
            strQuantityWhole = strQuantityWhole.mid(0,7);
        while (strQuantityWhole.length()<7)
            strQuantityWhole.insert(0,' ');
        comm = comm + strQuantityWhole;

        strQuantityFract = QString::number(infoRetDoc.quantityFract);
        if (strQuantityFract.length()>3)
            strQuantityFract = strQuantityFract.mid(0,3);
        while (strQuantityFract.length()<3)
            strQuantityFract.insert(0,'0');
        comm = comm + strQuantityFract;

        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);

        sendData(buf);
    }

    return Output;
}

bool QSentoLanE::clearGoodsBase()
{
    bool Output = false;

    startPacket packetInfo;
    packetInfo.taskType = 'i';
    packetInfo.transType = ttypePCtoCMClear;
    packetInfo.startNum = 6;

    Output = sendStartPacket(packetInfo);

    return Output;
}

bool QSentoLanE::readSerialNumber()
{
    bool Output = false;

    startExtandedPacket packetInfo;
    packetInfo.taskType = '1';
    packetInfo.subTask = 0x00;
    packetInfo.transType = ttypeCMtoPC;

    Output = sendStartExtendedPacket(packetInfo);

    if (Output)
        sendEOT();

    return Output;
}

bool QSentoLanE::printLine (quint8 number,QString line)
{
    bool Output = true;

    if ((currentDevice->isOpen())&&(number<=6))
    {
        QString comm;
        QByteArray buf;
        QString CRC;
        QString currentText;
        if ((number!=0)&&(number!=6))
        {
            while (line.length()>0)
            {
                currentDevice->waitForBytesWritten(1000);
                comm.clear();
                CRC.clear();
                comm = comm + "D*N";
                comm = comm + QString::number(number);
                currentText =  line.mid(0,33);
                line.remove(0,33);

                if (currentText.length()<33)
                    for (int i=currentText.length();i<33;i++)
                        currentText = currentText + " ";

                comm = comm +currentText;

                buf.clear();
                buf.push_back(toCP1251(&comm));
                buf.push_back(cbyteETX);
                CRC = QString::number(CalcCrc(&buf),16).right(4);
                while (CRC.length()<4) CRC.insert(0,'0');
                buf.push_back(CRC.toLatin1());
                buf.insert(0,cbyteSTX);
                sendData(buf);


            }

        }
        else
        {
            comm = comm + "D*N";
            comm = comm + QString::number(number);
            comm = comm + "                                 ";
            buf.clear();
            buf.push_back(toCP1251(&comm));
            buf.push_back(cbyteETX);
            CRC = QString::number(CalcCrc(&buf),16).right(4);
            while (CRC.length()<4) CRC.insert(0,'0');
            buf.push_back(CRC.toLatin1());
            buf.insert(0,cbyteSTX);
            sendData(buf);
        }



    } else Output = false;

    return Output;
}

bool QSentoLanE::readRolesOperators()
{
    bool Output = false;

    rolesOperators.clear();

    startExtandedPacket packetInfo;
    packetInfo.taskType = 'D';
    packetInfo.subTask = 0x02;
    packetInfo.transType = ttypeCMtoPC;

    Output = sendStartExtendedPacket(packetInfo);

    if (Output)
        sendEOT();

    return Output;
}

bool QSentoLanE::writeRoleOperator(roleOperator role)
{
    bool Output = false;

    if (currentDevice->isOpen())
    {
        QString comm;
        QByteArray buf;
        QString strNum;
        QString strName;
        QString strRight;

        QString CRC;

        comm = "DD#021";

        strNum = QString::number(role.num);
        if (strNum.length()>2)
            strNum = strNum.mid(0,2);
        if (strNum.length()<2)
            strNum.insert(0,'0');

        comm = comm + strNum;


        strName = role.name;
        if (strName.length()>20)
            strName = strName.mid(0,20);
        while (strName.length()<20)
            strName.push_back(' ');

        comm = comm + strName;

        quint64 currentRight = 0;
        quint8 *ptrCurrentRight = (quint8*)(&currentRight);
        quint8 *ptrRight = (quint8*)(&role.right);

        currentRight = rightsBitsToQuint64(role.right);

        strRight = QString::number(currentRight,16).right(16);

        while (strRight.length()<16)
            strRight.insert(0,'0');

        comm = comm + strRight;


        buf.clear();
        buf.push_back(toCP1251(&comm));
        buf.push_back(cbyteETX);
        CRC = QString::number(CalcCrc(&buf),16).right(4);
        while (CRC.length()<4) CRC.insert(0,'0');
        buf.push_back(CRC.toLatin1());
        buf.insert(0,cbyteSTX);
        sendData(buf);

    } else Output = false;

    return Output;
}

short QSentoLanE::CalcCrc(char *str,int len)
{
    short crc=0;
    for(int i=0;i<len;i++)
        crc ^= (short)((unsigned char)str[i]) << (i%9);
    return crc;
}

quint16 QSentoLanE::CalcCrc(QByteArray *str)
{
    quint16 crc=0;
    for(int i=0;i<str->length();i++)
        crc ^= (quint16)((unsigned char)str->at(i)) << (i%9);
    return crc;
}

QString QSentoLanE::fromCP1251 (QByteArray *input)
{
    QString Output;

    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    Output = codec->toUnicode(*input);
    return Output;
}

QByteArray QSentoLanE::toCP1251 (QString *input)
{
    QByteArray Output;

    QByteArray buf(input->toStdString().c_str());
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    Output = codec->fromUnicode(buf);

    return Output;
}

void QSentoLanE::slotReadyRead()
{
    qDebug()<<"QSentoLanE::slotReadyRead";
    inputBuffer.push_back(currentDevice->readAll());

    flagWhile = true;

    while ((inputBuffer.length()>0) && (flagWhile))
    {
        switch (inputBuffer.at(0)) {
        case cbyteENQ:
            {
                qDebug()<<"<ENQ>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
                eventENQ();
            }
            break;
        case cbyteACK:
            {
                qDebug()<<"<ACK>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
                eventACK();

            }
            break;
        case cbyteNAK:
            {
                qDebug()<<"<NAK>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
            }
            break;
        case cbyteETX:
            {
                qDebug()<<"<ETX>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
            }
            break;
        case cbyteETB:
            {
                qDebug()<<"<ETB>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
            }
            break;
        case cbyteEOT:
            {
                qDebug()<<"<EOT>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
                eventEOT();
            }
            break;
        case cbyteWACK:
            {
                qDebug()<<"<WACK>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
            }
            break;
        case cbyteRVI:
            {
                qDebug()<<"<RVI>";
                emit signalRxData(inputBuffer.mid(0,1));
                inputBuffer.remove(0,1);
                eventRVI();

            }
            break;
        case cbyteSTX:
            {
                qDebug()<<"<STX>";
                //emit signalRxData(inputBuffer);
                int pos = inputBuffer.indexOf(cbyteETB);
                if (pos<0)
                    pos = inputBuffer.indexOf(cbyteETX);

                if (pos>0)
                {
                    if ((pos+5)<=inputBuffer.length())
                    {
                        QByteArray packet = inputBuffer.mid(0,pos+5);
                        emit signalRxData(packet);
                        commandHandler(packet);
                        inputBuffer.remove(0,pos+5);

                        if (flagReceive)
                            sendACK();

                    } else flagWhile = false;
                } else flagWhile = false;
            }
            break;
        default:
            {
                qDebug()<<"Error first byte";
                inputBuffer.remove(0,1);
            }
            break;

        }
    }
}

void QSentoLanE::commandHandler(QByteArray command)
{
    command = command.mid(0,command.length()-4);
    command.remove(command.indexOf(cbyteSTX),1);

    QString strCommand = fromCP1251(&command);
    qDebug()<<"strCommad="<<strCommand;

    if (strCommand.indexOf("DD#000")>=0)
        CommandReadOperator(strCommand);

    if (strCommand.indexOf("D1#00")>=0)
        CommandReadSerialNumber(strCommand);

    if (strCommand.indexOf("D*")>=0)
        commandStatDoc(strCommand);

    if (strCommand.indexOf("DD#020")>=0)
        commandRolesOperators(strCommand);

}

void QSentoLanE::CommandReadOperator(QString strCommand)
{
    strCommand.remove("DD#000");

    operatorCM currentOperator;
    currentOperator.number = strCommand.mid(0,2).toInt();
    strCommand.remove(0,2);

    currentOperator.name = strCommand.mid(0,20);
    currentOperator.name .remove(" ");
    currentOperator.name .remove(currentOperator.name.length()-1,1);
    currentOperator.name = strCommand.remove(0,20);

    currentOperator.pasword = strCommand.mid(0,8).toInt();
    strCommand.remove(0,8);

    currentOperator.right = (opRigths)strCommand.mid(0,2).toInt();
    strCommand.remove(0,2);


    listOperators.push_back(currentOperator);

    if(strCommand.indexOf(cbyteETX)>=0)
        emit signalOperatorsList(listOperators);
}

void QSentoLanE::CommandReadSerialNumber(QString strCommand)
{
    strCommand.remove(cbyteETX);
    strCommand.remove("D1#00");

    emit signalSerialNumber(strCommand);
}

void QSentoLanE::commandStatDoc(QString strCommand)
{
    qDebug()<<"QSentoLanE::commandStatDoc";
    statDocument status;
    strCommand.remove(cbyteETX);
    strCommand.remove("D*");

    status.flagOpenDoc = strCommand.mid(0,1).toInt();
    strCommand.remove(0,1);

    status.sumWhole = strCommand.mid(0,8).toInt();
    strCommand.remove(0,8);

    status.sumFract = strCommand.mid(0,2).toInt();
    strCommand.remove(0,2);

    status.countItems = strCommand.mid(0,3).toInt();
    strCommand.remove(0,3);

    status.number = strCommand.mid(0,6).toInt();
    strCommand.remove(0,6);

    emit signalStatusDocument(status);
}

void QSentoLanE::commandRolesOperators(QString strCommand)
{
    qDebug()<<"QSentoLanE::commandRolesOperators";
    bool end = false;

    if(strCommand.indexOf(cbyteETX)>=0)
        end = true;

    strCommand.remove(cbyteSTX);
    strCommand.remove(cbyteETX);
    strCommand.remove("DD#020");


    roleOperator currentRole;
    currentRole.num = strCommand.mid(0,2).toInt();
    strCommand.remove(0,2);

    currentRole.name = strCommand.mid(0,20);
    currentRole.name.remove("\u0098");
    strCommand.remove(0,20);

    bool ok;
    quint64 currentRight = strCommand.mid(0,16).toULongLong(&ok,16);

    currentRole.right = rightsBitsFromQuint64(currentRight);

    rolesOperators.push_back(currentRole);

    if(end)
        emit signalRolesOperators(rolesOperators);
}

void QSentoLanE::slotTImerACK()
{

}

quint64 QSentoLanE::rightsBitsToQuint64 (rightsBits &input)
{
    quint64 Output = 0;

    if(input.Sale) Output = Output | maskSale;
    if(input.CancelPayDocs) Output = Output | maskCancelPayDocs;
    if(input.ServAttach) Output = Output | maskServAttach;
    if(input.DiscFromDB) Output = Output | maskDiscFromDB;
    if(input.DiscManual) Output = Output | maskDiscManual;
    if(input.SetReports) Output = Output | maskSetReports;
    if(input.SetZReports) Output = Output | maskSetZReports;
    if(input.RepZReport) Output = Output | maskRepZReport;
    if(input.RepXReport) Output = Output | maskRepXReport;
    if(input.RepBEP) Output = Output | maskRepBEP;
    if(input.KepFundsKSA) Output = Output | maskKepFundsKSA;
    if(input.KepZFundsKSA) Output = Output | maskKepZFundsKSA;
    if(input.RepAmend) Output = Output | maskRepAmend;
    if(input.RepCashier) Output = Output | maskRepCashier;
    if(input.RepZCashier) Output = Output | maskRepZCashier;
    if(input.RepSales) Output = Output | maskRepSales;
    if(input.RepZSales) Output = Output | maskRepZSales;
    if(input.BT) Output = Output | maskBT;
    if(input.ChangePrice) Output = Output | maskChangePrice;
    if(input.DiscountDB) Output = Output | maskDiscountDB;
    if(input.Cashiers) Output = Output | maskCashiers;
    if(input.PrintDB) Output = Output | maskPrintDB;
    if(input.ProgClock) Output = Output | maskProgClock;
    if(input.CurrencyProgII) Output = Output | maskCurrencyProgII;
    if(input.ProgOption) Output = Output | maskProgOption;
    if(input.ProgOtherOptions) Output = Output | maskProgOtherOptions;
    if(input.PCDataExch) Output = Output | maskPCDataExch;
    if(input.ModemDataExch) Output = Output | maskModemDataExch;
    if(input.EquipDataExch) Output = Output | maskEquipDataExch;
    if(input.FullMenu) Output = Output | maskFullMenu;
    if(input.DefDisc) Output = Output | maskDefDisc;
    if(input.ServExtrad) Output = Output | maskServExtrad;
    if(input.Correction) Output = Output | maskCorrection;
    if(input.SalesByName) Output = Output | maskSalesByName;
    if(input.ListedSales) Output = Output | maskListedSales;
    if(input.Orders) Output = Output | maskOrders;
    if(input.ConfFTP) Output = Output | maskConfFTP;
    if(input.FTPConnect) Output = Output | maskFTPConnect;
    if(input.OpenCashDrawer) Output = Output | maskOpenCashDrawer;
    if(input.ELFunctions) Output = Output | maskELFunctions;
    if(input.RepEH) Output = Output | maskRepEH;
    if(input.EHMemCard) Output = Output | maskEHMemCard;
    if(input.Return) Output = Output | maskReturn;
    if(input.CancelPolicy) Output = Output | maskCancelPolicy;

    return Output;
}

QSentoLanE::rightsBits QSentoLanE::rightsBitsFromQuint64 (quint64 &input)
{
    rightsBits Output;

    if (input & maskSale) Output.Sale = true;
    if (input & maskCancelPayDocs) Output.CancelPayDocs = true;
    if (input & maskServAttach) Output.ServAttach = true;
    if (input & maskDiscFromDB) Output.DiscFromDB = true;
    if (input & maskDiscManual) Output.DiscManual = true;
    if (input & maskSetReports) Output.SetReports = true;
    if (input & maskSetZReports) Output.SetZReports = true;
    if (input & maskRepZReport) Output.RepZReport = true;
    if (input & maskRepXReport) Output.RepXReport = true;
    if (input & maskRepBEP) Output.RepBEP = true;
    if (input & maskKepFundsKSA) Output.KepFundsKSA = true;
    if (input & maskKepZFundsKSA) Output.KepZFundsKSA = true;
    if (input & maskRepAmend) Output.RepAmend = true;
    if (input & maskRepCashier) Output.RepCashier = true;
    if (input & maskRepZCashier) Output.RepZCashier = true;
    if (input & maskRepSales) Output.RepSales = true;
    if (input & maskRepZSales) Output.RepZSales = true;
    if (input & maskBT) Output.BT = true;
    if (input & maskChangePrice) Output.ChangePrice = true;
    if (input & maskDiscountDB) Output.DiscountDB = true;
    if (input & maskCashiers) Output.Cashiers = true;
    if (input & maskPrintDB) Output.PrintDB = true;
    if (input & maskProgClock) Output.ProgClock = true;
    if (input & maskCurrencyProgII) Output.CurrencyProgII = true;
    if (input & maskProgOption) Output.ProgOption = true;
    if (input & maskProgOtherOptions) Output.ProgOtherOptions = true;
    if (input & maskPCDataExch) Output.PCDataExch = true;
    if (input & maskModemDataExch) Output.ModemDataExch = true;
    if (input & maskEquipDataExch) Output.EquipDataExch = true;
    if (input & maskFullMenu) Output.FullMenu = true;
    if (input & maskDefDisc) Output.DefDisc = true;
    if (input & maskServExtrad) Output.ServExtrad = true;
    if (input & maskCorrection) Output.Correction = true;
    if (input & maskSalesByName) Output.SalesByName = true;
    if (input & maskListedSales) Output.ListedSales = true;
    if (input & maskOrders) Output.Orders = true;
    if (input & maskConfFTP) Output.ConfFTP = true;
    if (input & maskFTPConnect) Output.FTPConnect = true;
    if (input & maskOpenCashDrawer) Output.OpenCashDrawer = true;
    if (input & maskELFunctions) Output.ELFunctions = true;
    if (input & maskRepEH) Output.RepEH = true;
    if (input & maskEHMemCard) Output.EHMemCard = true;
    if (input & maskReturn) Output.Return = true;
    if (input & maskCancelPolicy) Output.CancelPolicy = true;

   return Output;
}
