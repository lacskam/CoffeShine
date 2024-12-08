#include "qcoffeepriceinfo.h"

QCoffeePriceInfo::QCoffeePriceInfo()
{

}

/*
void QCoffeePriceInfo::operator =(QCoffeePriceInfo &input)
{
    id = input.id;
    value = input.value;
    startDate = input.startDate;
    description = input.description;
    idDrink = input.idDrink;
    idVolume = input.idVolume;
    idDrinkCategory = input.idDrinkCategory;

    idPointSale.clear();
    for (int i = 0;i<input.idPointSale.length();i++)
        idPointSale.push_back(input.idPointSale.at(i));
}
*/

void QCoffeePriceInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>value;
    stream>>startDate;
    stream>>description;
    stream>>idDrink;
    stream>>idVolume;
    stream>>idDrinkCategory;

    idPointSale.clear();
    quint16 count;
    stream >> count;
    int currentId;
    for (int i=0;i<count;i++){
        stream>>currentId;
        idPointSale.push_back(currentId);
    }
}

void QCoffeePriceInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << value;
    stream << startDate;
    stream << description;
    stream << idDrink;
    stream << idVolume;
    stream << idDrinkCategory;

    stream << (quint16)idPointSale.length();
    int currentId;
    for (int i=0;i<idPointSale.length();i++) {
        currentId = idPointSale.at(i);
        stream << currentId;
    }
}

QByteArray QCoffeePriceInfo::toByteArray()
{
    QByteArray Output;

    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<id;
    stream<<value;
    stream<<startDate;
    stream<<description;
    stream<<idDrink;
    stream<<idVolume;
    stream<<idDrinkCategory;

    stream<<idPointSale;

    return Output;
}

void QCoffeePriceInfo::fromByteArray(QByteArray data)
{
    if (data.length()>3)
    {

        QDataStream stream(&data,QIODevice::ReadOnly);
        stream.device()->seek(0);

        stream>>id;
        stream>>value;
        stream>>startDate;
        stream>>description;
        stream>>idDrink;
        stream>>idVolume;
        stream>>idDrinkCategory;

        stream>>idPointSale;
    }
}

QString QCoffeePriceInfo::toString()
{
    QString Output;


    Output =    "id: " + QString::number(id) +
                " value:" + QString::number(value) +
                " startDate:" + startDate.toString("yyyy-MM-dd hh:mm:ss") +
                " description:" + description +
                " idDrink:" + QString::number(idDrink) +
                " idVolume:" + QString::number(idVolume) +
                " idDrinkCategory:" + QString::number(idDrinkCategory);

    Output = Output + " idPointSale{";

    for (int i=0;i<idPointSale.length();i++)
        Output = Output + QString::number(idPointSale.at(i)) + " ";

    Output = Output + "}";

    return Output;
}
