#include "qcoffeesoldpositioninfo.h"

QCoffeeSoldPositionInfo::QCoffeeSoldPositionInfo()
{

}

/*
void QCoffeeSoldPositionInfo::operator =(QCoffeeSoldPositionInfo &input)
{
    id = input.id;
    idUser = input.idUser;
    count = input.count;
    idDrink = input.idDrink;
    idTypeDrink = input.idTypeDrink;
    idPrice = input.idPrice;
    idVolumeDrink = input.idVolumeDrink;
    idReceipt = input.idReceipt;
    idPointSale = input.idPointSale;
    numberInReceipt = input.numberInReceipt;
}
*/

void QCoffeeSoldPositionInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>idUser;
    stream>>count;
    stream>>idDrink;
    stream>>idTypeDrink;
    stream>>idPrice;
    stream>>idVolumeDrink;
    stream>>idReceipt;
    stream>>idPointSale;
    stream>>numberInReceipt;
    stream>>date;
}

void QCoffeeSoldPositionInfo::operator >>(QDataStream &stream)
{
    stream<<id;
    stream<<idUser;
    stream<<count;
    stream<<idDrink;
    stream<<idTypeDrink;
    stream<<idPrice;
    stream<<idVolumeDrink;
    stream<<idReceipt;
    stream<<idPointSale;
    stream<<numberInReceipt;
    stream<<date;

}

QByteArray QCoffeeSoldPositionInfo::toByteArray()
{
    QByteArray Output;

    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<id;
    stream<<idUser;
    stream<<count;
    stream<<idDrink;
    stream<<idTypeDrink;
    stream<<idPrice;
    stream<<idVolumeDrink;
    stream<<idReceipt;
    stream<<idPointSale;
    stream<<numberInReceipt;
    stream<<date;

    return Output;
}

void QCoffeeSoldPositionInfo::fromByteArray(QByteArray data)
{
    if (data.length()>3)
    {

        QDataStream stream(&data,QIODevice::ReadOnly);
        stream.device()->seek(0);

        stream>>id;
        stream>>idUser;
        stream>>count;
        stream>>idDrink;
        stream>>idTypeDrink;
        stream>>idPrice;
        stream>>idVolumeDrink;
        stream>>idReceipt;
        stream>>idPointSale;
        stream>>numberInReceipt;
        stream>>date;

    }
}

QString QCoffeeSoldPositionInfo::toString()
{
    QString Output;

    Output = Output + "id:" + QString::number(id);
    Output = Output + "idUser:" + QString::number(idUser);
    Output = Output + " count:" + QString::number(count);
    Output = Output + " idDrink:" + QString::number(idDrink);
    Output = Output + " idTypeDrink:" + QString::number(idTypeDrink);
    Output = Output + " idPrice:" + QString::number(idPrice);
    Output = Output + " idVolumeDrink:" + QString::number(idVolumeDrink);
    Output = Output + " idReceipt:" + QString::number(idReceipt);
    Output = Output + " idPointSale:" + QString::number(idPointSale);
    Output = Output + " numberInReceipt:" + QString::number(numberInReceipt);
    Output = Output + " date:" + date.toString("yyyy-MM-dd hh:mm:ss");

    return Output;
}
