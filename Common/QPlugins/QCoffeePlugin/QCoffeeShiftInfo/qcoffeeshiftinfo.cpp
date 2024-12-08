#include "qcoffeeshiftinfo.h"

QCoffeeShiftInfo::QCoffeeShiftInfo()
{
    id = -1;
    open = false;
    close = false;

}

void QCoffeeShiftInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>openTime;
    stream>>closeTime;
    stream>>open;
    stream>>close;
    stream>>idUser;
    stream>>idPointSale;
}

void QCoffeeShiftInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << openTime;
    stream << closeTime;
    stream << open;
    stream << close;
    stream << idUser;
    stream << idPointSale;
}

QByteArray QCoffeeShiftInfo::toByteArray()
{
    QByteArray Output;

    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<id;
    stream<<openTime;
    stream<<closeTime;
    stream<<open;
    stream<<close;
    stream<<idUser;
    stream<<idPointSale;


    return Output;
}

void QCoffeeShiftInfo::fromByteArray(QByteArray data)
{
    if (data.length()>3)
    {
        QDataStream stream(&data,QIODevice::ReadOnly);
        stream.device()->seek(0);

        stream>>id;
        stream>>openTime;
        stream>>closeTime;
        stream>>open;
        stream>>close;
        stream<<idUser;
        stream<<idPointSale;
    }
}

QString QCoffeeShiftInfo::toString()
{
    QString Output;

    Output = "id:" + QString::number(id) +
             " openTime:" + openTime.toString("yyyy-MM-dd hh:mm:ss") +
             " closeTime:" + closeTime.toString("yyyy-MM-dd hh:mm:ss") +
             " open:" +QString::number(open) +
             " close:" +QString::number(close) +
             " idUser:" +QString::number(idUser) +
             " idPointSale:" +QString::number(idPointSale);


    return Output;
}
