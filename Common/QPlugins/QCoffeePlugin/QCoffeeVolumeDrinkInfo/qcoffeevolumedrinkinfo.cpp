#include "qcoffeevolumedrinkinfo.h"

QCoffeeVolumeDrinkInfo::QCoffeeVolumeDrinkInfo()
{

}

/*
void QCoffeeVolumeDrinkInfo::operator =(QCoffeeVolumeDrinkInfo &input)
{
    id = input.id;
    name = input.name;
    volume = input.volume;
    description = input.description;

    idDrink.clear();
    for (int i=0;input.idDrink.length();i++)
        idDrink.push_back(input.idDrink.at(i));
}
*/

void QCoffeeVolumeDrinkInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>name;
    stream>>volume;
    stream>>description;
    stream>>units;

    idDrink.clear();
    quint16 count;
    stream >> count;
    int currentId;
    for (int i=0;i<count;i++){
        stream>>currentId;
        idDrink.push_back(currentId);
    }
}

void QCoffeeVolumeDrinkInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << name;
    stream << volume;
    stream << description;
    stream << units;

    stream << (quint16)idDrink.length();
    int currentId;
    for (int i=0;i<idDrink.length();i++) {
        currentId = idDrink.at(i);
        stream << currentId;
    }
}

QByteArray QCoffeeVolumeDrinkInfo::toByteArray()
{
    QByteArray Output;

    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<id;
    stream<<name;
    stream<<volume;
    stream<<description;
    stream<<idDrink;


    return Output;
}

void QCoffeeVolumeDrinkInfo::fromByteArray(QByteArray data)
{
    if (data.length()>3)
    {

        QDataStream stream(&data,QIODevice::ReadOnly);
        stream.device()->seek(0);

        stream>>id;
        stream>>name;
        stream>>volume;
        stream>>description;
        stream>>idDrink;
    }
}

QString QCoffeeVolumeDrinkInfo::toString()
{
    QString Output;


    Output = "id: " + QString::number(id) + " name:" + name +
             " volume:" + QString::number(volume) + " description:" + description;

    Output = Output + " idDrink{";

    for (int i=0;i<idDrink.length();i++)
        Output = Output + QString::number(idDrink.at(i)) + " ";

    Output = Output + "}";

    return Output;
}
