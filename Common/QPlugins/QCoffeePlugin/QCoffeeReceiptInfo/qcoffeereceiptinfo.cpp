#include "qcoffeereceiptinfo.h"

QCoffeeReceiptInfo::QCoffeeReceiptInfo()
{
    id = -1;
}

void QCoffeeReceiptInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>dateTime;
    stream>>sum;
    stream>>idPointSale;
    stream>>idShift;
}

void QCoffeeReceiptInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << dateTime;
    stream << sum;
    stream << idPointSale;
    stream << idShift;

}

QByteArray QCoffeeReceiptInfo::toByteArray()
{
    QByteArray Output;

    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<id;
    stream<<dateTime;
    stream<<sum;
    stream<<idPointSale;
    stream<<idShift;

    return Output;
}

void QCoffeeReceiptInfo::fromByteArray(QByteArray data)
{
    if (data.length()>3)
    {
        QDataStream stream(&data,QIODevice::ReadOnly);
        stream.device()->seek(0);

        stream>>id;
        stream>>dateTime;
        stream>>sum;
        stream>>idPointSale;
        stream>>idShift;
    }
}

QString QCoffeeReceiptInfo::toString()
{
    QString Output;

    Output = "id: " + QString::number(id) + " data:" + dateTime.toString("yyyy-MM-dd hh:mm:ss") +
             " sum:" + QString::number(sum) + " idPointSale:" + QString::number(idPointSale) + " "
             " idShift:" + QString::number(idShift);

    return Output;
}
